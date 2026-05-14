#pragma once

#include "SDL3/SDL_events.h"

using Keycode = SDL_Keycode;
using Scancode = SDL_Scancode;

namespace lili {

enum class EventType {
	NONE,
	QUIT,
	WINDOW_RESIZED,
	KEYBOARD,
	MOUSE_BUTTON,
	MOUSE_MOTION,
	MOUSE_WHEEL
};

enum class KeyAction {
	PRESSED,
	RELEASED
};

enum class MouseButton {
	UNKNOWN,
	LEFT,
	MIDDLE,
	RIGHT
};

enum class MouseAction {
	PRESSED,
	RELEASED
};

struct KeyboardEvent {
	Keycode key;
	Scancode scancode;
	KeyAction action;
	bool repeat;
};

struct MouseButtonEvent {
	MouseButton button;
	MouseAction action;
	float x;
	float y;
};

struct MouseMotionEvent {
	float x;
	float y;
	float dx;
	float dy;
};

struct MouseWheelEvent {
	float dx;
	float dy;
};

class Event {
public:
	Event();
	~Event() = default;

	bool poll();
	EventType type() const;

	bool key_just_pressed(const KeyboardEvent &keyboard) const;
	bool mouse_button_pressed(const MouseButtonEvent &keyboard) const;

	const KeyboardEvent keyboard() const;
	const MouseButtonEvent mouse_button() const;
	const MouseMotionEvent mouse_motion() const;
	const MouseWheelEvent mouse_wheel() const;

private:
	SDL_Event sdl_event;
	EventType current_type;

	EventType resolve_type() const;
};

}  // namespace lili
