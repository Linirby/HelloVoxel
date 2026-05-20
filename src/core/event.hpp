/// SDL event wrapper with typed accessors.
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
	/// Create an event wrapper.
	Event();
	/// Destroy the event wrapper.
	~Event() = default;

	/// Poll the next SDL event and update cached state.
	bool poll();
	/// Get the current event type.
	EventType type() const;

	/// Check if a keyboard event is a fresh press.
	bool key_just_pressed(const KeyboardEvent &keyboard) const;
	/// Check if a mouse button event is a press.
	bool mouse_button_pressed(const MouseButtonEvent &keyboard) const;

	/// Get the current keyboard event.
	const KeyboardEvent keyboard() const;
	/// Get the current mouse button event.
	const MouseButtonEvent mouse_button() const;
	/// Get the current mouse motion event.
	const MouseMotionEvent mouse_motion() const;
	/// Get the current mouse wheel event.
	const MouseWheelEvent mouse_wheel() const;

private:
	SDL_Event sdl_event;
	EventType current_type;

	EventType resolve_type() const;
};

}  // namespace lili
