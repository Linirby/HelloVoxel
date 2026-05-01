#ifndef APP_HPP
# define APP_HPP

# include <memory>
# include <SDL3/SDL.h>

# include "core/sdl_system.hpp"
# include "core/window.hpp"

# include "physics/collision.hpp"

# include "render/passes/renderer.hpp"
# include "render/scene/camera.hpp"
# include "render/scene/model.hpp"
# include "render/scene/sprite.hpp"

# include "entity/player.hpp"

struct ChunkRenderData {
	std::unique_ptr<lili::GPUMesh> mesh = nullptr;
	std::unique_ptr<lili::Model> model = nullptr;
	lili::Mat4 transform;
};

class App {
public:
	void run(const std::string &map_path = "assets/maps/test_01.json");

private:
	// Core
	std::unique_ptr<lili::SDLSystem> sdl_sys = nullptr;
	std::unique_ptr<lili::Window> window = nullptr;
	std::unique_ptr<lili::Renderer> renderer = nullptr;

	// Settings
	int win_w = 1280;
	int win_h = 720;
	float fov_y = 90.0f;
	
	// Resources
	std::string map_path = "assets/maps/test_01.json";
	lili::Map map;
	lili::Texture *atlas = nullptr;
	std::unordered_map<uint64_t, ChunkRenderData> chunk_models;

	lili::Player player;
	lili::RaycastResult player_raycast;
	lili::Camera camera;

	std::unique_ptr<lili::Sprite> crosshair = nullptr;
	
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
