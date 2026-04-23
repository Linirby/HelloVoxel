#include <stdexcept>

#include "app.hpp"
#include "map_loader.hpp"
#include "geometry/block.hpp"

void App::run() {
	init_core();
	init_resources();
	mainloop();
	cleanup();
}

void App::init_core() {
	if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());

	window = SDL_CreateWindow("HelloVoxel", win_w, win_h, 0);
	if (!window) throw std::runtime_error(SDL_GetError());

	if (!SDL_SetWindowRelativeMouseMode(window, true)) {
		throw std::runtime_error(SDL_GetError());
	}
	renderer = new lili::Renderer(window);
	if (!renderer) throw std::runtime_error("Renderer creation failed!");
	is_running = true;
}

void App::init_resources() {
	camera = lili::Camera(-90.0f, 0.0f, fov_y);
	map = load_map("assets/maps/test_01.json", player);
	atlas = new lili::Texture(renderer->get_device(), "assets/cube_atlas.png");
	if (!atlas) throw std::runtime_error("Atlas texture creation failed!");
	for (const auto &pair : map.chunks)
		update_chunk_mesh(pair.first);
	crosshair = new lili::Quad(renderer->get_device(), "assets/crosshair.png");
}

void App::update_chunk_mesh(uint64_t key) {
	auto chunk_it = chunk_models.find(key);
	if (chunk_it != chunk_models.end()) {
		if (chunk_it->second.model) {
			delete chunk_it->second.model;
			chunk_models.erase(chunk_it);
		}
	}

	lili::MeshData chunk_data = lili::ChunkMesher::generate_mesh(
		map.chunks[key]
	);
	if (chunk_data.vertices.empty()) {
		auto model_it = chunk_models.find(key);
		if (model_it != chunk_models.end()) {
			delete model_it->second.model;
			chunk_models.erase(model_it);
		}
		return;
	}

	auto model_it = chunk_models.find(key);
	if (model_it != chunk_models.end())
		if (model_it->second.model) delete model_it->second.model;

	lili::GPUMesh *chunk_mesh = new lili::GPUMesh(
		renderer->get_device(), chunk_data
	);
	lili::Model *chunk_model = new lili::Model(chunk_mesh, atlas);

	int chunk_x = static_cast<int16_t>(key >> 32);
	int chunk_y = static_cast<int16_t>(key >> 16);
	int chunk_z = static_cast<int16_t>(key);

	lili::Mat4 transform = lili::Mat4::translate({
		static_cast<float>(chunk_x * lili::Chunk::SIZE),
		static_cast<float>(chunk_y * lili::Chunk::SIZE),
		static_cast<float>(chunk_z * lili::Chunk::SIZE)
	});
	chunk_models[key] = ChunkRenderData{ chunk_model, transform };
}

void App::handle_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) is_running = false;
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_ESCAPE) is_running = false;

			if (event.key.key == SDLK_P) player.toggle_spectator();
			if (event.key.key == SDLK_B) player.toggle_builder();

			if (event.key.key == SDLK_R) {
				if (renderer) SDL_WaitForGPUIdle(renderer->get_device());
				cleanup_resources();
				init_resources();
			}
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
			camera.process_mouse(event.motion.xrel, event.motion.yrel);
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
			if (player.mode != lili::PlayerMode::Builder) return;
			uint8_t handed_block = lili::BLOCK_ID_DEBUG;
			if (event.button.button == SDL_BUTTON_LEFT) {
				uint8_t old_block = map.get_block_global(
					player_raycast.hit_x,
					player_raycast.hit_y,
					player_raycast.hit_z
				);
				if (old_block == 0) return;
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
					player_raycast.adjacent_x,
					player_raycast.adjacent_y,
					player_raycast.adjacent_z
				);
				if (old_block == 1) return;
				map.set_block_global(
					handed_block,
					player_raycast.adjacent_x,
					player_raycast.adjacent_y,
					player_raycast.adjacent_z
				);
				update_chunk_mesh(map.get_chunk_key(
					player_raycast.adjacent_x >> 4,
					player_raycast.adjacent_y >> 4,
					player_raycast.adjacent_z >> 4
				));
			}
		}
	}
}

void App::update(float dt) {
	const bool *keys = SDL_GetKeyboardState(NULL);

	if (player.mode == lili::PlayerMode::Builder)
		player_raycast = map.raycast(
			camera.position, camera.front, player.build_range 
		);
	player.process_keys(keys, camera.front, camera.right, camera.up, dt);
	player.update_physics(dt, map);

	camera.position = player.position;
	if (player.mode != lili::PlayerMode::Spectator) camera.position.y += 1.6f;
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

	int win_w = 0;
	int win_h = 0;
	SDL_GetWindowSize(window, &win_w, &win_h);
	crosshair->position = { win_w / 2.0f, win_h / 2.0f, 0.0f };
	crosshair->scale = { 18.0f, 18.0f, 1.0f };
	crosshair->draw(renderer);

	renderer->end_frame();
}

void App::mainloop() {
	Uint64 last = SDL_GetTicks();

	while (is_running) {
		Uint64 now = SDL_GetTicks();
		float dt = (now - last) / 1000.0f;
		last = now;
		
		handle_events();
		update(dt);
		render();
	}
}

void App::cleanup_resources() {
	if (crosshair) delete crosshair;
	for (const auto &data : chunk_models) {
		if (data.second.model) delete data.second.model;
	}
	chunk_models.clear();
	if (atlas) delete atlas;
}

void App::cleanup_core() {
	if (renderer) delete renderer;
	if (window) SDL_DestroyWindow(window);
}

void App::cleanup() {
	if (renderer) {
		SDL_WaitForGPUIdle(renderer->get_device());
	}

	cleanup_resources();
	cleanup_core();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
