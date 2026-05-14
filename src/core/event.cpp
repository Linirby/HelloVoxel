#include "core/event.hpp"

namespace lili {

Event::Event() {
	sdl_event = SDL_Event();
	current_type = EventType::NONE;
}

bool Event::poll() {
	if (SDL_PollEvent(&sdl_event)) {
		current_type = resolve_type();
		return true;
	}
	return false;
}

EventType Event::type() const {
	return current_type;
}

bool Event::key_just_pressed(const KeyboardEvent &keyboard) const {
	return keyboard.action == KeyAction::PRESSED && !keyboard.repeat;
}

bool Event::mouse_button_pressed(const MouseButtonEvent &mouse_button) const {
	return mouse_button.action == MouseAction::PRESSED;
}

const KeyboardEvent Event::keyboard() const {
	return {
		.key = sdl_event.key.key,
		.scancode = sdl_event.key.scancode,
		.action = (sdl_event.type == SDL_EVENT_KEY_DOWN) ?
			KeyAction::PRESSED : KeyAction::RELEASED,
		.repeat = sdl_event.key.repeat
	};
}

const MouseButtonEvent Event::mouse_button() const {
	MouseButton button;
	switch (sdl_event.button.button) {
		case SDL_BUTTON_LEFT:
			button = MouseButton::LEFT;
			break;
		case SDL_BUTTON_MIDDLE:
			button = MouseButton::MIDDLE;
			break;
		case SDL_BUTTON_RIGHT:
			button = MouseButton::RIGHT;
			break;
		default:
			button = MouseButton::UNKNOWN;
			break;
	}
	return {
		.button = button,
		.action = (sdl_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ?
			MouseAction::PRESSED : MouseAction::RELEASED,
		.x = sdl_event.button.x,
		.y = sdl_event.button.y
	};
}

const MouseMotionEvent Event::mouse_motion() const {
	return {
		.x = sdl_event.motion.x,
		.y = sdl_event.motion.y,
		.dx = sdl_event.motion.xrel,
		.dy = sdl_event.motion.yrel
	};
}

const MouseWheelEvent Event::mouse_wheel() const {
	return {
		.dx = sdl_event.wheel.x,
		.dy = sdl_event.wheel.y
	};
}

EventType Event::resolve_type() const {
	switch (sdl_event.type) {
		case SDL_EVENT_QUIT: return EventType::QUIT;
		case SDL_EVENT_WINDOW_RESIZED: return EventType::WINDOW_RESIZED;

		case SDL_EVENT_KEY_DOWN:
		case SDL_EVENT_KEY_UP: return EventType::KEYBOARD;

		case SDL_EVENT_MOUSE_BUTTON_DOWN:
		case SDL_EVENT_MOUSE_BUTTON_UP: return EventType::MOUSE_BUTTON;

		case SDL_EVENT_MOUSE_MOTION: return EventType::MOUSE_MOTION;
		case SDL_EVENT_MOUSE_WHEEL: return EventType::MOUSE_WHEEL;
		default: return EventType::NONE;
	}
}

}  // namespace lili
