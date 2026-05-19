#pragma once

#include <memory>

#include "render/renderer.hpp"
#include "render/scene/ui_text.hpp"

namespace lili {

class Clock {
public:
	Clock();
	~Clock() = default;

	void set_fixed_dt(float value);

	void update();
	bool step();
	const float get_fixed_dt() const;
	const float get_dt() const;
	const float get_alpha() const;
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
