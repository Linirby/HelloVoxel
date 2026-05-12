#pragma once

#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "geometry/mat4x4.hpp"

namespace lili {

class DirectionalLight {
public:
	DirectionalLight();
	~DirectionalLight() = default;

	void set_direction(const Vec3 &dir);
	void set_color(const Vec4 &rgba);
	void set_shadow_distance(float value);
	void set_orthogonal_half(float value);
	void update_focus(const Vec3 &target_point);

	Vec3 get_direction() const;
	Vec4 get_color() const;
	Mat4 get_projection_view() const;

private:
	Vec3 direction;
	Vec4 color;
	float shadow_distance;
	float orthogonal_half;
	Vec3 focus_point;
	Vec3 position;

	Mat4 view;
	Mat4 projection;
	Mat4 projection_view;
};

}  // namespace lili
