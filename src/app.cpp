#include <iostream>

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
	player = lili::Player({
		.position = { 0.5f, 3.0, 0.5f },
	});
	camera = lili::Camera(-90.0f, 0.0f, fov_y);
	map = lili::load_map(map_path);

	atlas = std::make_unique<lili::Texture>(
		renderer->get_device(), "assets/cube_atlas.png"
	);
	world_material = std::make_unique<lili::Material>(atlas.get());
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
}

void App::update_chunk_mesh(uint64_t key) {
	lili::MeshData chunk_data = lili::ChunkMesher::generate_mesh(
		map.chunks[key]
	);
	if (chunk_data.vertices.empty()) return;
	auto chunk_mesh = std::make_unique<lili::GPUMesh>(
		renderer->get_device(), chunk_data
	);
	auto chunk_model = std::make_unique<lili::Model>(
		chunk_mesh.get(), world_material.get()
	);

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
				init_resources();
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
			uint8_t handed_block = lili::BLOCK_ID_DEBUG;
			if (event.button.button == SDL_BUTTON_LEFT) {
				uint8_t old_block = map.get_block_global(
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
				if (old_block == 0) continue;
				map.set_block_global(
					lili::BLOCK_ID_AIR,
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
				update_chunk_mesh(map.get_chunk_key(
					player_raycast.hit_x >> 4,
					player_raycast.hit_y >> 4,
					player_raycast.hit_z >> 4
				));
			}
			if (event.button.button == SDL_BUTTON_RIGHT) {
				uint8_t old_block = map.get_block_global(
					player_raycast.adj_x,
					player_raycast.adj_y,
					player_raycast.adj_z
				);
				if (old_block == 1) continue;
				map.set_block_global(
					handed_block,
					player_raycast.adj_x,
					player_raycast.adj_y,
					player_raycast.adj_z
				);
				update_chunk_mesh(map.get_chunk_key(
					player_raycast.adj_x >> 4,
					player_raycast.adj_y >> 4,
					player_raycast.adj_z >> 4
				));
			}
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) {
			std::array<int, 2> win_size = window->get_size();
			renderer->on_window_resized(win_size[0], win_size[1]);
			crosshair->position = { win_size[0] / 2.0f, win_size[1] / 2.0f, 0 };
		}
	}
}

void App::update(float dt) {
	camera.position = player.position;
	if (player.mode != lili::PlayerMode::Spectator) camera.position.y += 1.6f;
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
