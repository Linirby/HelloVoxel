#include <iostream>
#include <stdexcept>
#include <vector>

#include "app.hpp"

#include "core/window.hpp"
#include "world/map_manager.hpp"
#include "world/block.hpp"
#include "meshing/mesher.hpp"

void App::run(const std::string &map_path) {
	this->map_path = map_path;
	init_core();
	init_resources();
	mainloop();
}

void App::init_core() {
	sdl_sys = std::make_unique<lili::SDLSystem>();
	window = std::make_unique<lili::Window>("HelloVoxel", win_w, win_h);
	window->set_relative_mouse_mode(true);
	renderer = std::make_unique<lili::Renderer>(window.get());
	is_running = true;
}

void App::init_resources() {
	clear_world_render_cache();

	player = lili::Player({
		.position = { 0.5f, 3.0, 0.5f },
	});
	camera = lili::Camera(-90.0f, 0.0f, fov_y);
	map = lili::load_map(map_path);

	atlas = std::make_unique<lili::Texture>(
		renderer->get_device(), "assets/cube_atlas.png"
	);
	if (!atlas) throw std::runtime_error("Failed to init atlas");
	world_material = std::make_unique<lili::Material>(atlas.get());
	if (!world_material)
		throw std::runtime_error("Failed to init world material");
	world_material->properties.color_tint = { 1.0f, 0.9f, 0.8f, 1.0f };
	world_material->properties.roughness = 0.8f;

	for (const auto &pair : map.chunks)
		update_chunk_mesh(pair.first);
	
	std::array<int, 2> win_size = window->get_size();
	crosshair = std::make_unique<lili::Sprite>(
		renderer->get_device(),
		"assets/crosshair.png",
		(lili::Vec3){ win_size[0] / 2.0f, win_size[1] / 2.0f, 0.0f },
		(lili::Vec3){ 18.0f, 18.0f, 1.0f },
		(lili::Vec3){ 0.0f, 0.0f, 0.0f }
	);
	if (!crosshair) throw std::runtime_error("Failed to init crosshair sprite");

	font = std::make_unique<lili::BitmapFont>(
		renderer->get_device(), "assets/lili_font.png"
	);
	fps_text = std::make_unique<lili::UIText>(
		renderer->get_device(), font.get(), "FPS:Loading"
	);
}

void App::clear_world_render_cache() {
	chunk_models.clear();
}

void App::update_chunk_mesh(uint64_t key) {
	auto chunk_it = map.chunks.find(key);
	if (chunk_it == map.chunks.end()) {
		chunk_models.erase(key);
		return;
	}
	lili::MeshData chunk_data = lili::ChunkMesher::generate_mesh(chunk_it->second);
	if (chunk_data.vertices.empty()) {
		chunk_models.erase(key);
		return;
	}
	auto chunk_mesh = std::make_unique<lili::GPUMesh>(
		renderer->get_device(), chunk_data
	);
	if (!chunk_mesh) throw std::runtime_error("Failed to create chunk GPUMesh");
	auto chunk_model = std::make_unique<lili::Model>(
		chunk_mesh.get(), world_material.get()
	);
	if (!chunk_model) throw std::runtime_error("Failed to create chunk model");

	int chunk_x = static_cast<int16_t>(key >> 32);
	int chunk_y = static_cast<int16_t>(key >> 16);
	int chunk_z = static_cast<int16_t>(key);

	lili::Mat4 transform = lili::Mat4::translate({
		static_cast<float>(chunk_x * lili::Chunk::SIZE),
		static_cast<float>(chunk_y * lili::Chunk::SIZE),
		static_cast<float>(chunk_z * lili::Chunk::SIZE)
	});
	chunk_models[key] = ChunkRenderData{
		std::move(chunk_mesh), std::move(chunk_model), transform
	};
}

void App::remesh_chunks_affected_by_block(int x, int y, int z) {
	const int chunk_x = x >> 4;
	const int chunk_y = y >> 4;
	const int chunk_z = z >> 4;
	const int local_x = x & 15;
	const int local_y = y & 15;
	const int local_z = z & 15;

	std::vector<uint64_t> keys;
	auto push_unique_key = [&](int cx, int cy, int cz) {
		const uint64_t key = map.get_chunk_key(cx, cy, cz);
		for (const uint64_t existing_key : keys) {
			if (existing_key == key) return;
		}
		keys.push_back(key);
	};

	push_unique_key(chunk_x, chunk_y, chunk_z);
	if (local_x == 0) push_unique_key(chunk_x - 1, chunk_y, chunk_z);
	if (local_x == 15) push_unique_key(chunk_x + 1, chunk_y, chunk_z);
	if (local_y == 0) push_unique_key(chunk_x, chunk_y - 1, chunk_z);
	if (local_y == 15) push_unique_key(chunk_x, chunk_y + 1, chunk_z);
	if (local_z == 0) push_unique_key(chunk_x, chunk_y, chunk_z - 1);
	if (local_z == 15) push_unique_key(chunk_x, chunk_y, chunk_z + 1);

	for (const uint64_t key : keys) {
		update_chunk_mesh(key);
	}
}

void App::handle_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) is_running = false;
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_ESCAPE) is_running = false;
			if (event.key.key == SDLK_TAB) {
				bool is_relative = window->is_relative_mouse_mode();
				window->set_relative_mouse_mode(!is_relative);
			}

			if (event.key.key == SDLK_P) player.toggle_spectator();
			if (event.key.key == SDLK_B) player.toggle_builder();

			if (event.key.key == SDLK_R) {
				if (renderer) SDL_WaitForGPUIdle(renderer->get_device());
				player = lili::Player({ .position = { 0.5f, 3.0, 0.5f } });
				camera = lili::Camera(-90.0f, 0.0f, fov_y);
			}

			const bool *keys = SDL_GetKeyboardState(NULL);
			if (keys[SDL_SCANCODE_LCTRL] && keys[SDL_SCANCODE_S]) {
				lili::save_map("custom_map.json", map);
				std::cout << "Map saved at custom_map.json" << '\n';
				return;
			}
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
			if (window->is_relative_mouse_mode())
				camera.process_mouse(event.motion.xrel, event.motion.yrel);
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (player.mode != lili::PlayerMode::Builder) continue;
			if (!player_raycast.hit) continue;
			uint8_t handed_block = lili::BLOCK_ID_LOG;
			if (event.button.button == SDL_BUTTON_LEFT) {
				uint8_t old_block = map.get_block_global(
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
				if (old_block == lili::BLOCK_ID_AIR) continue;
				map.set_block_global(
					lili::BLOCK_ID_AIR,
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
				remesh_chunks_affected_by_block(
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				uint8_t old_block = map.get_block_global(
					player_raycast.adj_x,
					player_raycast.adj_y,
					player_raycast.adj_z
				);
				if (old_block != lili::BLOCK_ID_AIR) continue;
				map.set_block_global(
					handed_block,
					player_raycast.adj_x,
					player_raycast.adj_y,
					player_raycast.adj_z
				);
				remesh_chunks_affected_by_block(
					player_raycast.adj_x,
					player_raycast.adj_y,
					player_raycast.adj_z
				);
			}
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			std::array<int, 2> win_size = window->get_size();
			win_w = win_size[0];
			win_h = win_size[1];      
			renderer->on_window_resized(win_w, win_h);
			crosshair->position = { win_w / 2.0f, win_h / 2.0f, 0 };
		}
	}
}

void App::update(float dt) {
	camera.position = player.position;
	if (player.mode != lili::PlayerMode::Spectator) camera.position.y += 1.6f;

	const lili::Vec3 light_dir = lili::Vec3{ -0.5f, -1.0f, -0.3f }.normalized();
	const float shadow_dist = 96.0f;
	const float ortho_half = 36.0f;
	const lili::Vec3 scene_center = camera.position;
	const lili::Vec3 light_pos = scene_center - light_dir * shadow_dist;

	const lili::Mat4 light_view = lili::Mat4::look_at(
		light_pos, scene_center, lili::Vec3{ 0.0f, 1.0f, 0.0f }
	);
	const lili::Mat4 light_proj = lili::Mat4::orthographic(
		-ortho_half, ortho_half,
		ortho_half, -ortho_half,
		0.1f, shadow_dist * 2.0f
	);
	renderer->set_light_matrix(light_proj * light_view);

	if (second_counter <= 1.0f) {
		second_counter += dt;
		temp_fps++;
	} else {
		fps = temp_fps;
		fps_text->set_text("FPS:" + std::to_string(fps));
		second_counter = 0.0f;
		temp_fps = 0;
	}
}

void App::fixed_update(float dt) {
    const bool *keys = SDL_GetKeyboardState(NULL);

    if (player.mode == lili::PlayerMode::Builder) {
        player_raycast = lili::raycast_voxel(
            camera.position, camera.front, player.build_range, map 
        );
    }
    player.process_keys(keys, camera.front, camera.right, camera.up, dt);
    player.update_physics(dt, map);

    camera.position = player.position;
    if (player.mode != lili::PlayerMode::Spectator) {
        camera.position.y += 1.6f;
    }
}

void App::render() {
	if (!renderer->begin_frame(camera)) return;

	for (const auto &data : chunk_models) {
		renderer->submit(
			*data.second.model,
			data.second.transform,
			lili::RenderLayer::World3D
		);
	}
	crosshair->draw(renderer.get());
	fps_text->draw(renderer.get(), (lili::Vec3){ 16.0f, win_h - 16.0f, 0.0f });

	renderer->end_frame();
}

void App::mainloop() {
    Uint64 last = SDL_GetTicks();

    const float fixed_dt = 1.0f / 60.0f; 
    float accumulator = 0.0f;

    while (is_running) {
        Uint64 now = SDL_GetTicks();
        float frame_time = (now - last) / 1000.0f;
        last = now;

        if (frame_time > 0.25f) {
            frame_time = 0.25f;
        }
        accumulator += frame_time;
        handle_events();
        while (accumulator >= fixed_dt) {
            fixed_update(fixed_dt); 
            accumulator -= fixed_dt;
        }
		update(frame_time);
        render();
    }
}
