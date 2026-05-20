#include "core/keyboard.hpp"

#include <cstring>

namespace lili {

Keyboard::Keyboard() {
	current = nullptr;
	std::memset(previous, 0, sizeof(previous));
	key_count = 0;
}

void Keyboard::update() {
	if (current)
		std::memcpy(previous, current, key_count * sizeof(bool));
	current = SDL_GetKeyboardState(&key_count);
	if (key_count > SDL_SCANCODE_COUNT)
		key_count = SDL_SCANCODE_COUNT;
}

bool Keyboard::held(Scancode key) const {
	return current && key < key_count && current[key];
}

bool Keyboard::pressed(Scancode key) const {
	return current && key < key_count && current[key] && !previous[key];
}

bool Keyboard::released(Scancode key) const {
	return current && key < key_count && !current[key] && previous[key];
}

}  // namespace lili
