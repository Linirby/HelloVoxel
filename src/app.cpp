#include <stdexcept>
#include "app.hpp"
#include "geometry/block.hpp"

static lili::Chunk load_chunk() {
	lili::Chunk chunk;

	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			chunk.set_block(lili::BLOCK_ID_DEBUG, x, 0, z);
		}
	}
	for (int y = 0; y < lili::Chunk::SIZE; ++y) {
		chunk.set_block(lili::BLOCK_ID_DEBUG, 0, y, 0);
		chunk.set_block(lili::BLOCK_ID_DEBUG, lili::Chunk::SIZE - 1, y, 0);
		chunk.set_block(lili::BLOCK_ID_DEBUG, 0, y, lili::Chunk::SIZE - 1);
		chunk.set_block(
			lili::BLOCK_ID_DEBUG,
			lili::Chunk::SIZE - 1,
			y,
			lili::Chunk::SIZE - 1
		);
	}
	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			chunk.set_block(lili::BLOCK_ID_DEBUG, x, lili::Chunk::SIZE - 1, z);
		}
	}
	return chunk;
}

void App::run() {
	init_window();
	core.renderer = new lili::Renderer(core.window);

	res.player = {
		{
			static_cast<int>(lili::Chunk::SIZE / 2),
			static_cast<int>(lili::Chunk::SIZE / 2),
			30.0f
		},
		10.0f
	};

	lili::MeshData chunk_data = lili::ChunkMesher::generate_mesh(load_chunk());
	lili::GPUMesh *chunk_mesh = new lili::GPUMesh(
		core.renderer->get_device(), chunk_data
	);
	if (!chunk_mesh) throw std::runtime_error("Buffer creation failed!");

	lili::Texture *atlas = new lili::Texture(
		core.renderer->get_device(), "assets/cube_atlas.png"
	);
	if (!atlas) throw std::runtime_error("Atlas texture creation failed!");

	res.chunk_models.push_back(new lili::Model(chunk_mesh, atlas));

	mainloop();
	cleanup();
}

void App::init_window() {
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
	is_running = true;
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
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION) {
			res.camera.process_mouse(event.motion.xrel, event.motion.yrel);
		}
	}
}

void App::update(float dt) {
	const bool *keys = SDL_GetKeyboardState(NULL);

	res.player.process_keyboard(
		keys, res.camera.front, res.camera.right, res.camera.up, dt
	);
	res.camera.position = res.player.position;
}

void App::render() {
	core.renderer->begin_frame(res.camera);

	for (lili::Model *chunk_model : res.chunk_models)
		core.renderer->draw(*chunk_model, lili::Mat4::identity());

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

void App::cleanup() {
	for (lili::Model *chunk_model : res.chunk_models)
		delete chunk_model;
	res.chunk_models.clear();
	if (core.renderer) delete core.renderer;
	if (core.window) SDL_DestroyWindow(core.window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
