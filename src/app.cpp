#include <stdexcept>

#include "app.hpp"
#include "map_loader.hpp"

#include "geometry/block.hpp"
#include "geometry/primitives.hpp"

static lili::Chunk load_chunk() {
	lili::Chunk chunk;

	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			chunk.set_block(lili::BLOCK_ID_DEBUG, x, 0, z);
		}
	}
	for (int y = 0; y < lili::Chunk::SIZE; ++y) {
		chunk.set_block(lili::BLOCK_ID_DEBUG, 0, y, 7);
		chunk.set_block(lili::BLOCK_ID_DEBUG, lili::Chunk::SIZE - 1, y, 8);
	}
	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			chunk.set_block(lili::BLOCK_ID_DEBUG, x, lili::Chunk::SIZE - 1, z);
		}
	}

	return chunk;
}

void App::run() {
	init_core();
	init_resources();
	mainloop();
	cleanup();
}

void App::init_core() {
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		throw std::runtime_error(
			"SDL_Init() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	core.window = SDL_CreateWindow(
		"HelloVoxel", 1280, 720, 0
	);
	if (!core.window) {
		throw std::runtime_error(
			"core.window creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	SDL_SetWindowRelativeMouseMode(core.window, true);
	core.renderer = new lili::Renderer(core.window);
	is_running = true;
}

void App::init_resources() {
	if (core.renderer) {
		SDL_WaitForGPUIdle(core.renderer->get_device());
	}

	if (res.crosshair_model) delete res.crosshair_model;
	if (res.crosshair_texture) delete res.crosshair_texture;
	for (
		const std::pair<const uint64_t, ChunkRenderData> &data :
		res.chunk_models
	) {
		delete data.second.model;
	}
	res.chunk_models.clear();
	if (res.atlas) delete res.atlas;

	res.camera = lili::Camera(-90.0f, 0.0f);
	res.atlas = new lili::Texture(
		core.renderer->get_device(), "assets/cube_atlas.png"
	);
	if (!res.atlas) throw std::runtime_error("Atlas texture creation failed!");
	res.map = lili::load_map("assets/maps/test_01.json", res.player);

	for (const std::pair<const uint64_t, lili::Chunk> &pair : res.map.chunks) {
		update_chunk_mesh(pair.first);
	}

	lili::MeshData quad_data = lili::create_unit_quad();
	lili::GPUMesh *quad_mesh = new lili::GPUMesh(
		core.renderer->get_device(), quad_data
	);
	if (!quad_mesh) throw std::runtime_error("GPU Mesh creation failed!");
	res.crosshair_texture = new lili::Texture(
		core.renderer->get_device(), "assets/crosshair.png"
	);
	if (!res.crosshair_texture)
		throw std::runtime_error("Crosshair texture creation failed!");
	res.crosshair_model = new lili::Model(quad_mesh, res.crosshair_texture);
}

void App::update_chunk_mesh(uint64_t key) {
	auto chunk_it = res.chunk_models.find(key);
	if (chunk_it != res.chunk_models.end()) {
		if (chunk_it->second.model) {
			delete chunk_it->second.model;
			res.chunk_models.erase(chunk_it);
		}
	}

	lili::MeshData chunk_data = lili::ChunkMesher::generate_mesh(
		res.map.chunks[key]
	);
	if (chunk_data.vertices.empty()) {
		auto model_it = res.chunk_models.find(key);
		if (model_it != res.chunk_models.end()) {
			delete model_it->second.model;
			res.chunk_models.erase(model_it);
		}
		return;
	}

	auto model_it = res.chunk_models.find(key);
	if (model_it != res.chunk_models.end()) {
		if (model_it->second.model) delete model_it->second.model;
	}

	lili::GPUMesh *chunk_mesh = new lili::GPUMesh(
		core.renderer->get_device(), chunk_data
	);
	lili::Model *chunk_model = new lili::Model(chunk_mesh, res.atlas);

	int chunk_x = static_cast<int16_t>(key >> 32);
	int chunk_y = static_cast<int16_t>(key >> 16);
	int chunk_z = static_cast<int16_t>(key);

	lili::Mat4 transform = lili::Mat4::translate({
		static_cast<float>(chunk_x * lili::Chunk::SIZE),
		static_cast<float>(chunk_y * lili::Chunk::SIZE),
		static_cast<float>(chunk_z * lili::Chunk::SIZE)
	});
	res.chunk_models[key] = ChunkRenderData{ chunk_model, transform };
}

void App::handle_events() {
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT) {
			is_running = false;
		}
		if (event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_ESCAPE) {
				is_running = false;
			}
			if (event.key.key == SDLK_P) {
				res.player.toggle_spectator();
			}
			if (event.key.key == SDLK_B) {
				res.player.toggle_builder();
			}
			if (event.key.key == SDLK_R) {
				init_resources();
			}
			if (event.key.key == SDLK_G) {
				static bool block_place = false;
				if (block_place) {
					res.map.set_block_global(0, 1, 2, -1);
					block_place = false;
				} else {
					res.map.set_block_global(1, 1, 2, -1);
					block_place = true;
				}
				update_chunk_mesh(res.map.get_chunk_key(
					10 >> 4, 5 >> 4, -10 >> 4
				));
			}
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION) {
			res.camera.process_mouse(event.motion.xrel, event.motion.yrel);
		}
	}
}

void App::update(float dt) {
	const bool *keys = SDL_GetKeyboardState(NULL);

	res.player.process_input(
		keys, res.camera.front, res.camera.right, res.camera.up, dt
	);
	res.player.update_physics(dt, res.map);
	res.camera.position = res.player.position;
	if (res.player.mode != lili::PlayerMode::Spectator) {
		res.camera.position.y += 1.6f;
	}
	// std::cout << "Player Position:" << '\n';
	// std::cout << "  x: " << res.player.position.x << '\n';
	// std::cout << "  z: " << res.player.position.z << '\n';
}

void App::render() {
	core.renderer->begin_frame(res.camera);

	for (
		const std::pair<const uint64_t, ChunkRenderData> &data :
		res.chunk_models
	) {
		core.renderer->submit(
			*data.second.model,
			data.second.transform,
			lili::RenderLayer::World3D
		);
	}

	int win_w = 0;
	int win_h = 0;
	SDL_GetWindowSize(core.window, &win_w, &win_h);
	lili::Mat4 translation = lili::Mat4::translate(
		{ win_w / 2.0f, win_h / 2.0f, 0.0f }
	);
	lili::Mat4 scale = lili::Mat4::scale({ 18.0f, 18.0f, 1.0f });
	lili::Mat4 transformation = translation * scale;
	core.renderer->submit(
		*res.crosshair_model, transformation, lili::RenderLayer::UI2D
	);

	core.renderer->end_frame();
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
	if (res.crosshair_model) delete res.crosshair_model;
	if (res.crosshair_texture) delete res.crosshair_texture;
	for (
		const std::pair<const uint64_t, ChunkRenderData> &data :
		res.chunk_models
	) {
		delete data.second.model;
	}
	res.chunk_models.clear();
	if (res.atlas) delete res.atlas;
}

void App::cleanup_core() {
	if (core.renderer) delete core.renderer;
	if (core.window) SDL_DestroyWindow(core.window);
}

void App::cleanup() {
	if (core.renderer) {
		SDL_WaitForGPUIdle(core.renderer->get_device());
	}

	cleanup_resources();
	cleanup_core();
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
