/// Fixed-timestep clock with FPS tracking.
#pragma once

#include <memory>

#include "render/renderer.hpp"
#include "render/scene/ui_text.hpp"

namespace lili {

class Clock {
public:
	/// Create a fixed-timestep clock.
	Clock();
	/// Destroy the clock.
	~Clock() = default;

	/// Set the fixed timestep duration in seconds.
	void set_fixed_dt(float value);

	/// Update internal timers for the current frame.
	void update();
	/// Advance fixed steps while available.
	bool step();
	/// Get the fixed timestep duration in seconds.
	const float get_fixed_dt() const;
	/// Get the last frame delta time in seconds.
	const float get_dt() const;
	/// Get the interpolation alpha between fixed steps.
	const float get_alpha() const;
	/// Draw the current FPS counter.
	void draw_fps(Renderer *renderer, BitmapFont *font, const Vec3 pos);

private:
	uint64_t last;
	uint64_t now;

	float fixed_dt;
	float dt;
	float accumulator;
	float second_counter;

	int fps;
	int temp_fps;
	std::unique_ptr<UIText> fps_text;
};

}  // namespace lili
