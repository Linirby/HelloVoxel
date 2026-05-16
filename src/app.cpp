#include "app.hpp"

#include <iostream>
#include <stdexcept>

void App::run(const std::string &map_path) {
	this->map_path = map_path;
	init_core();
	init_resources();
	mainloop();
}

void App::init_core() {
	sdl_sys = std::make_unique<lili::SDLSystem>();

	window = std::make_unique<lili::Window>("HelloVoxel", win_w, win_h);
	window->set_relative_mouse_mode(true);
	renderer = std::make_unique<lili::Renderer>(window.get());

	is_running = true;
}

void App::init_resources() {
	player = lili::Player();
	player.set_position({ 0.5f, 3.0f, 0.5f });
	camera = lili::Camera(-90.0f, 0.0f, fov_y);
	player.bind_camera(camera);

	world = std::make_unique<lili::WorldRuntime>();
	world->bind_renderer(renderer.get());
	world->set_material_albedo("assets/cube_atlas.png");
	world->set_material_properties({
		.color_tint = { 1.0f, 0.9f, 0.8f, 1.0f },
		.roughness = 0.8f
	});
	world->load_map_path(map_path);
	world->load_chunks();
	
	std::array<int, 2> win_size = window->get_size();
	crosshair = std::make_unique<lili::Sprite>(
		renderer->get_device(),
		"assets/crosshair.png",
		(lili::Vec3){ win_size[0] / 2.0f, win_size[1] / 2.0f, 0.0f },
		(lili::Vec3){ 18.0f, 18.0f, 1.0f },
		(lili::Vec3){ 0.0f, 0.0f, 0.0f }
	);
	if (!crosshair) throw std::runtime_error("Failed to init crosshair sprite");

	font = std::make_unique<lili::BitmapFont>(
		renderer->get_device(), "assets/lili_font.png"
	);

}

void App::handle_inputs() {
	while (event.poll()) {
		if (event.type() == lili::EventType::QUIT) is_running = false;

		if (event.type() == lili::EventType::WINDOW_RESIZED) {
			std::array<int, 2> win_size = window->get_size();
			win_w = win_size[0];
			win_h = win_size[1];
			renderer->on_window_resized(win_w, win_h);
			crosshair->position = { win_w / 2.0f, win_h / 2.0f, 0 };
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
		camera = lili::Camera{ -90.0f, 0.0f, fov_y };
		player.bind_camera(camera);
	}
	if (
		keyboard.pressed(SDL_SCANCODE_LCTRL) &&
		keyboard.pressed(SDL_SCANCODE_S)
	) {
		world->save_map_file("custom_map.json");
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

	world->draw_chunks();
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
