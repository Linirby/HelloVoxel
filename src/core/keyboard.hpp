/// Keyboard input state wrapper.
#pragma once

#include <SDL3/SDL_events.h>

namespace lili {

using Scancode = SDL_Scancode;

class Keyboard {
public:
	/// Create a keyboard input tracker.
	Keyboard();
	/// Destroy the keyboard tracker.
	~Keyboard() = default;

	/// Update keyboard state for the current frame.
	void update();
	/// Check if a key is currently held.
	bool held(Scancode key) const;
	/// Check if a key was pressed this frame.
	bool pressed(Scancode key) const;
	/// Check if a key was released this frame.
	bool released(Scancode key) const;

private:
	const bool *current;	
	bool previous[SDL_SCANCODE_COUNT];
	int key_count;
};

}  // namespace lili
