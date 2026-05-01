#include "core/event.hpp"

namespace lili {

bool Event::process() {
	keyboard_state = SDL_GetKeyboardState(nullptr);
	mouse_state = SDL_GetMouseState(&mouse.x, &mouse.y);
	mouse.motion_xrel = event.motion.xrel;
	mouse.motion_yrel = event.motion.yrel;
	return SDL_PollEvent(&event);
}

bool Event::quit_button() {
	return event.type == SDL_EVENT_QUIT;
}

bool Event::is_key_pressed(int key) {
	return keyboard_state[key];
}

bool Event::is_mouse_pressed(int button) {
	return SDL_BUTTON_MASK(button);
}

const bool *Event::get_keyboard_state() {
	return keyboard_state;
}

std::array<float, 2> Event::get_mouse_pos() {
	return { mouse.x, mouse.y };
}

};
