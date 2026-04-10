#include <iostream>
#include "app.hpp"

void App::run() {
	init_window();
	core.renderer = new lili::Renderer(core.window);
	std::cout << "Renderer initialized!" << '\n';
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
	std::cout << "Window initialized!" << '\n';
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
		core.renderer->draw_chunk();
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
	if (core.renderer) delete core.renderer;
	if (core.window) SDL_DestroyWindow(core.window);
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}
