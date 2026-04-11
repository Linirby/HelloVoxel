#ifndef APP_HPP
# define APP_HPP

# include <SDL3/SDL.h>
# include <vector>

# include "render/renderer.hpp"
# include "render/camera.hpp"
# include "render/model.hpp"

# include "entity/player.hpp"

class App {
public:
	void run();

private:
	struct Core {
		SDL_Window *window = nullptr;
		lili::Renderer *renderer = nullptr;
	} core;
	struct Resources {
		std::vector<lili::Model *> chunk_models;
		lili::Camera camera;
		lili::Player player;
	} res;
	bool is_running = false;

	void init_window();
	void init_device();
	void init_textures();
	void init_chunk();
	void init_buffers();
	void init_shaders();
	void init_graphics_pipeline();

	void handle_events();
	void update(float dt);
	void render();
	
	void mainloop();
	void cleanup();
};

#endif
