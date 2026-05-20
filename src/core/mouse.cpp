#include "core/mouse.hpp"

namespace lili {

Mouse::Mouse() {
	current = 0;
	previous = 0;
	current_x = 0.0f;
	current_y = 0.0f;
	current_dx = 0.0f;
	current_dy = 0.0f;
}

void Mouse::update() {
	previous = current;
	current_dx = 0.0f;
	current_dy = 0.0f;
	current = SDL_GetMouseState(&current_x, &current_y);
	SDL_GetRelativeMouseState(&current_dx, &current_dy);
}

bool Mouse::held(MouseButton button) const {
	if (button == MouseButton::LEFT)
		return current & SDL_BUTTON_LMASK;
	else if (button == MouseButton::MIDDLE)
		return current & SDL_BUTTON_MMASK;
	else if (button == MouseButton::RIGHT)
		return current & SDL_BUTTON_RMASK;
	else
		return false;
}

bool Mouse::pressed(MouseButton button) const {
	if (button == MouseButton::LEFT)
		return (current & SDL_BUTTON_LMASK) && !(previous & SDL_BUTTON_LMASK);
	else if (button == MouseButton::MIDDLE)
		return (current & SDL_BUTTON_MMASK) && !(previous & SDL_BUTTON_MMASK);
	else if (button == MouseButton::RIGHT)
		return (current & SDL_BUTTON_RMASK) && !(previous & SDL_BUTTON_RMASK);
	else
		return false;
}

bool Mouse::released(MouseButton button) const {
	if (button == MouseButton::LEFT)
		return !(current & SDL_BUTTON_LMASK) && (previous & SDL_BUTTON_LMASK);
	else if (button == MouseButton::MIDDLE)
		return !(current & SDL_BUTTON_MMASK) && (previous & SDL_BUTTON_MMASK);
	else if (button == MouseButton::RIGHT)
		return !(current & SDL_BUTTON_RMASK) && (previous & SDL_BUTTON_RMASK);
	else
		return false;
}

float Mouse::get_x() const {
	return current_x;
}

float Mouse::get_y() const {
	return current_y;
}

float Mouse::get_dx() const {
	return current_dx;
}

float Mouse::get_dy() const {
	return current_dy;
}

}  // namespace lili
