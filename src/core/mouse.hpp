/// Mouse input state wrapper.
#pragma once

#include "core/event.hpp"

namespace lili {

class Mouse {
public:
	/// Create a mouse input tracker.
	Mouse();
	/// Destroy the mouse tracker.
	~Mouse() = default;

	/// Update mouse state for the current frame.
	void update();

	/// Check if a mouse button is currently held.
	bool held(MouseButton button) const;
	/// Check if a mouse button was pressed this frame.
	bool pressed(MouseButton button) const;
	/// Check if a mouse button was released this frame.
	bool released(MouseButton button) const;

	/// Get the current mouse X position.
	float get_x() const;
	/// Get the current mouse Y position.
	float get_y() const;
	/// Get the mouse X delta for this frame.
	float get_dx() const;
	/// Get the mouse Y delta for this frame.
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
