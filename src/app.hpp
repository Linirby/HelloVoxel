#ifndef APP_HPP
# define APP_HPP

# include <SDL3/SDL.h>

# include "render/renderer.hpp"
# include "render/camera.hpp"
# include "render/model.hpp"

# include "entity/player.hpp"

struct ChunkRenderData {
	lili::Model *model;
	lili::Mat4 transform;
};

class App {
public:
	void run();

private:
	struct Core {
		SDL_Window *window = nullptr;
		lili::Renderer *renderer = nullptr;
	} core;
	struct Resources {
		lili::Map map;
		lili::Texture *atlas = nullptr;
		std::vector<ChunkRenderData> chunk_models;

		lili::Texture *crosshair_texture = nullptr;
		lili::Model *crosshair_model = nullptr;

		lili::Player player;
		lili::Camera camera;
	} res;
	bool is_running = false;

	void init_core();
	void init_resources();

	void handle_events();
	void update(float dt);
	void render();
	
	void mainloop();

	void cleanup_resources();
	void cleanup_core();
	void cleanup();
};

#endif
