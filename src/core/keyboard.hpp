#pragma once

#include <SDL3/SDL_events.h>

namespace lili {

using Scancode = SDL_Scancode;

class Keyboard {
public:
	Keyboard();
	~Keyboard() = default;

	void update();
	bool held(Scancode key) const;
	bool pressed(Scancode key) const;
	bool released(Scancode key) const;

private:
	const bool *current;	
	bool previous[SDL_SCANCODE_COUNT];
	int key_count;
};

}  // namespace lili
