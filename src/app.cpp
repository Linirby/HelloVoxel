#include <iostream>

#include "app.hpp"
#include "geometry/block.hpp"

void App::run() {
	init_window();
	core.renderer = new lili::Renderer(core.window);
	init_chunk();
	lili::MeshData chunk_data = lili::ChunkMesher::generate_mesh(res.chunk);
	res.chunk_mesh = new lili::GPUMesh(core.renderer->get_device(), chunk_data);
	if (!res.chunk_mesh) {
		throw std::runtime_error("Buffer creation failed!");
	}
	res.atlas = new lili::Texture(
		core.renderer->get_device(), "assets/cube_atlas.png"
	);
	if (!res.atlas) {
		throw std::runtime_error("Atlas texture creation failed!");
	}
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

void App::init_chunk() {
	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			res.chunk.set_block(lili::BLOCK_ID_DEBUG, x, 0, z);
		}
	}
	for (int y = 0; y < lili::Chunk::SIZE; ++y) {
		res.chunk.set_block(lili::BLOCK_ID_DEBUG, 0,  y, 0);
		res.chunk.set_block(lili::BLOCK_ID_DEBUG, 0,  y, 15);
		res.chunk.set_block(lili::BLOCK_ID_DEBUG, 15, y, 0);
		res.chunk.set_block(lili::BLOCK_ID_DEBUG, 15, y, 15);
	}
	for (int x = 0; x < lili::Chunk::SIZE; ++x) {
		for (int z = 0; z < lili::Chunk::SIZE; ++z) {
			res.chunk.set_block(lili::BLOCK_ID_DEBUG, x, 15, z);
		}
	}
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
	if (core.renderer->begin_frame(res.camera)) {
		core.renderer->draw(res.chunk_mesh, res.atlas, lili::Mat4::identity());
		core.renderer->draw(
			res.chunk_mesh,
			res.atlas,
			lili::Mat4::translate({ 16.0f, 0.0f, 0.0f })
		);
		core.renderer->draw(
			res.chunk_mesh,
			res.atlas,
			lili::Mat4::translate({ -16.0f, 0.0f, 0.0f })
		);
		core.renderer->draw(
			res.chunk_mesh,
			res.atlas,
			lili::Mat4::translate({ 0.0f, 0.0f, 16.0f })
		);
		core.renderer->draw(
			res.chunk_mesh,
			res.atlas,
			lili::Mat4::translate({ 0.0f, 0.0f, -16.0f })
		);
		core.renderer->end_frame();
	} else {
		std::cout << "Can't begin frame!" << '\n';
	}
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
	if (res.atlas) delete res.atlas;
	if (res.chunk_mesh) delete res.chunk_mesh;
	if (core.renderer) delete core.renderer;
	if (core.window) SDL_DestroyWindow(core.window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
