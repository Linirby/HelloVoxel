#include <iostream>
#include <stdexcept>
#include <vector>

#include "app.hpp"

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

	player = lili::Player();
	player.set_position({ 0.5f, 3.0f, 0.5f });
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
	if (!crosshair) throw std::runtime_error("Failed to init crosshair sprite");

	font = std::make_unique<lili::BitmapFont>(
		renderer->get_device(), "assets/lili_font.png"
	);

	dir_light = std::make_unique<lili::DirectionalLight>();
	dir_light->set_direction({ -0.5f, -1.0f, -0.5f });
	dir_light->set_color({ 1.0f, 0.9f, 0.9f, 0.75f });
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

void App::place_block() {

}

void App::break_block() {

}

void App::handle_inputs() {
	while (event.poll()) {
		if (event.type() == lili::EventType::QUIT) is_running = false;

		if (event.type() == lili::EventType::WINDOW_RESIZED) {
			std::array<int, 2> win_size = window->get_size();
			win_w = win_size[0];
			win_h = win_size[1];
			renderer->on_window_resized(win_w, win_h);
			crosshair->position = { win_w / 2.0f, win_h / 2.0f, 0 };
		}
	}
	
	if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
		is_running = false;
	if (keyboard.pressed(SDL_SCANCODE_TAB)) {
		bool is_relative = window->is_relative_mouse_mode();
		window->set_relative_mouse_mode(!is_relative);
	}
	if (keyboard.pressed(SDL_SCANCODE_P))
		player.toggle_spectator();
	if (keyboard.pressed(SDL_SCANCODE_B))
		player.toggle_builder();

	if (keyboard.pressed(SDL_SCANCODE_R)) {
		if (renderer) SDL_WaitForGPUIdle(renderer->get_device());
		player.set_position({ 0.5f, 3.0f, 0.5f });
		camera = lili::Camera{ -90.0f, 0.0f, fov_y };
	}
	if (
		keyboard.pressed(SDL_SCANCODE_LCTRL) &&
		keyboard.pressed(SDL_SCANCODE_S)
	) {
		lili::save_map("custom_map.json", map);
		std::cout << "Map saved at: `custom_map.json`\n";
	}

    if (player.get_mode() == lili::PlayerMode::Builder) {
        player_raycast = lili::raycast_voxel(
            camera.position, camera.front, player.get_build_range(), map 
        );
		if (player_raycast.hit) {
			uint8_t handed_block = lili::BLOCK_ID_LOG;
			if (mouse.pressed(lili::MouseButton::LEFT)) {
				uint8_t old_block = map.get_block_global(
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
				if (old_block != lili::BLOCK_ID_AIR) {
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
			}
			if (mouse.pressed(lili::MouseButton::RIGHT)) {
				uint8_t old_block = map.get_block_global(
					player_raycast.adj_x,
					player_raycast.adj_y,
					player_raycast.adj_z
				);
				if (old_block == lili::BLOCK_ID_AIR) {
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
		}
    }

    player.process_keys(keyboard, camera);
}

void App::update(float dt) {
	if (window->is_relative_mouse_mode())
		camera.process_mouse(mouse.get_dx(), mouse.get_dy());

	camera.position = player.get_position();
	camera.position.y += 1.6f;

	dir_light->update_focus(camera.position);
	renderer->set_directional_light(dir_light.get());
}

void App::fixed_update(float dt) {
    player.update_physics(dt, map);
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
	clock.draw_fps(renderer.get(), font.get(), { 16.0f, win_h - 16.0f, 0.0f });

	renderer->end_frame();
}

void App::mainloop() {
    while (is_running) {
		clock.update();
		keyboard.update();
		mouse.update();

        handle_inputs();
        while (clock.step()) {
            fixed_update(clock.get_fixed_dt()); 
        }
		update(clock.get_dt());
        render();
    }
}
