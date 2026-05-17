#include "app.hpp"

#include <iostream>

#include "world/block.hpp"
#include "world/material.hpp"

void App::run(const std::string &map_path) {
	this->map_path = map_path;
	init_core();
	init_resources();
	mainloop();
}

void App::init_core() {
	sdl_sys = std::make_unique<lili::SDLSystem>();

	window = std::make_unique<lili::Window>();
	window->set_title("Voxel Template");
	window->set_size(1280, 720);
	window->set_resizable(true);
	window->set_relative_mouse_mode(true);

	renderer = std::make_unique<lili::Renderer>();

	is_running = true;
}

void App::init_resources() {
	lili::MaterialRegistry &material_registry = lili::MaterialRegistry::get();
	lili::Material custom_log_mat;
	custom_log_mat.properties.color_tint = { 0.9f, 1.0f, 0.8f, 1.0f };
	material_registry.register_material("custom:log_mat", custom_log_mat);

	lili::BlockRegistry &block_registry = lili::BlockRegistry::get();
	block_registry.register_block("custom:log", {
		.top_texture = 8,
		.bottom_texture = 8,
		.front_texture = 9,
		.right_texture = 9,
		.back_texture = 9,
		.left_texture = 9,
		.material_id = material_registry.get_material_id("custom:log_mat")
	});
	// After window is linked to the register no material can be add without a
	// new pass to update material list for the GPU
	renderer->set_window(window.get());

	camera = lili::Camera();
	camera.set_rotation(-90.0f, 0.0f);
	camera.set_fov(90.0f);

	player = lili::Player();
	player.set_position({ 0.5f, 3.0f, 0.5f });
	player.set_camera(camera);
	player.set_selected_block(block_registry.get_block_id("custom:log"));

	world = std::make_unique<lili::WorldRuntime>();
	world->set_atlas_map(renderer.get(), "assets/cube_atlas.png", 8, 8);
	world->load_map(map_path);
	
	crosshair = std::make_unique<lili::Sprite>();
	crosshair->set_texture(renderer.get(), "assets/crosshair.png");
	crosshair->set_position({ win_w / 2.0f, win_h / 2.0f, 0.0f });
	crosshair->set_scale({ 18.0f, 18.0f, 1.0f });
	crosshair->set_rotation({ 0.0f, 0.0f, 0.0f });

	font = std::make_unique<lili::BitmapFont>();
	font->set_atlas_map(renderer.get(), "assets/lili_font.png");
	font->set_size(16, 6);
}

void App::handle_inputs() {
	while (event.poll()) {
		if (event.type() == lili::EventType::QUIT) is_running = false;

		if (event.type() == lili::EventType::WINDOW_RESIZED) {
			std::array<int, 2> win_size = window->get_size();
			win_w = win_size[0];
			win_h = win_size[1];
			renderer->on_window_resized(win_w, win_h);
			crosshair->set_position({ win_w / 2.0f, win_h / 2.0f, 0.0f });
		}
	}
	
	if (keyboard.pressed(SDL_SCANCODE_ESCAPE))
		is_running = false;
	if (keyboard.pressed(SDL_SCANCODE_TAB)) {
		bool is_relative = window->is_relative_mouse_mode();
		window->set_relative_mouse_mode(!is_relative);
	}
	if (keyboard.pressed(SDL_SCANCODE_P))
		player.toggle_spectator();
	if (keyboard.pressed(SDL_SCANCODE_B))
		player.toggle_builder();

	if (keyboard.pressed(SDL_SCANCODE_R)) {
		if (renderer) SDL_WaitForGPUIdle(renderer->get_device());
		player.set_position({ 0.5f, 3.0f, 0.5f });
		camera = lili::Camera();
		camera.set_rotation(-90.0f, 0.0f);
		camera.set_fov(90.0f);
		player.set_camera(camera);
	}
	if (
		keyboard.pressed(SDL_SCANCODE_LCTRL) &&
		keyboard.pressed(SDL_SCANCODE_S)
	) {
		world->save_map("custom_map.json");
		std::cout << "Map saved at: `custom_map.json`\n";
	}

    if (player.get_mode() == lili::PlayerMode::Builder)
		player.process_mouse(mouse, world.get());

    player.process_keys(keyboard);
}

void App::update(float dt) {
	if (window->is_relative_mouse_mode())
		camera.process_mouse(mouse.get_dx(), mouse.get_dy());

	camera.position = player.get_position();
	camera.position.y += 1.6f;
}

void App::fixed_update(float dt) {
    player.update_physics(dt, world->get_map());
}

void App::render() {
	if (!renderer->begin_frame(camera)) return;

	world->draw_map();
	crosshair->draw(renderer.get());
	clock.draw_fps(renderer.get(), font.get(), { 16.0f, win_h - 16.0f, 0.0f });

	renderer->end_frame();
}

void App::mainloop() {
    while (is_running) {
		clock.update();
		keyboard.update();
		mouse.update();

        handle_inputs();
        while (clock.step()) {
            fixed_update(clock.get_fixed_dt()); 
        }
		update(clock.get_dt());
        render();
    }
}
