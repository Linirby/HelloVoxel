#pragma once

#include "core/event.hpp"

namespace lili {

class Mouse {
public:
	Mouse();
	~Mouse() = default;

	void update();

	bool held(MouseButton button) const;
	bool pressed(MouseButton button) const;
	bool released(MouseButton button) const;

	float get_x() const;
	float get_y() const;
	float get_dx() const;
	float get_dy() const;

private:
	uint32_t current;
	uint32_t previous;
	float current_x;
	float current_y;
	float current_dx;
	float current_dy;
};

}  // namespace lili
