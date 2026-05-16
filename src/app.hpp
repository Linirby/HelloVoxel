#pragma once

#include <memory>
#include <SDL3/SDL.h>

#include "core/sdl_system.hpp"
#include "core/window.hpp"
#include "core/clock.hpp"
#include "core/event.hpp"
#include "core/keyboard.hpp"
#include "core/mouse.hpp"

#include "physics/collision.hpp"

#include "render/renderer.hpp"
#include "render/scene/camera.hpp"
#include "render/scene/model.hpp"
#include "render/scene/sprite.hpp"
#include "render/scene/ui_text.hpp"

#include "entity/player.hpp"

#include "runtime/world_runtime.hpp"

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

	lili::Clock clock;
	lili::Event event;
	lili::Keyboard keyboard;
	lili::Mouse mouse;

	bool is_running = false;

	// Settings
	int win_w = 1280;
	int win_h = 720;
	float fov_y = 90.0f;
	
	// Resources
	std::string map_path = "assets/maps/test_01.json";
	std::unique_ptr<lili::Texture> atlas = nullptr;
	std::unique_ptr<lili::WorldRuntime> world = nullptr;

	lili::Player player;
	lili::RaycastResult player_raycast;
	lili::Camera camera;
	std::unique_ptr<lili::Sprite> crosshair = nullptr;
	std::unique_ptr<lili::BitmapFont> font = nullptr;

	void init_core();
	void init_resources();

	void place_block(uint8_t new_block);
	void break_block();

	void handle_inputs();
	void handle_keyboard();
	void update(float dt);
	void fixed_update(float dt);
	void render();
	void mainloop();
};
