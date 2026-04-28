#ifndef APP_HPP
# define APP_HPP

# include <SDL3/SDL.h>

# include "render/passes/renderer.hpp"
# include "render/scene/camera.hpp"
# include "render/scene/model.hpp"
# include "entity/player.hpp"

struct ChunkRenderData {
	lili::Model *model = nullptr;
	lili::Mat4 transform;
};

class App {
public:
	void run(const std::string &map_path = "assets/maps/test_01.json");

private:
	std::string map_path = "assets/maps/test_01.json";

	// Core
	SDL_Window *window = nullptr;
	lili::Renderer *renderer = nullptr;

	// Settings
	int win_w = 1280;
	int win_h = 720;
	float fov_y = 90.0f;
	
	// Resources
	lili::Map map;
	lili::Texture *atlas = nullptr;
	std::unordered_map<uint64_t, ChunkRenderData> chunk_models;

	lili::Player player;
	lili::RaycastResult player_raycast;
	lili::Camera camera;

	lili::Texture *crosshair_texture = nullptr;
	lili::GPUMesh *crosshair_mesh = nullptr;
	lili::Model *crosshair = nullptr;
	
	bool is_running = false;

	void init_core();
	void init_resources();

	void update_chunk_mesh(uint64_t key);

	void handle_events();
	void update(float dt);
	void fixed_update(float dt);
	void render();
	void mainloop();

	void cleanup_resources();
	void cleanup_core();
	void cleanup();
};

#endif  // APP_HPP
