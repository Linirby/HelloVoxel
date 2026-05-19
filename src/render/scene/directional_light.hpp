/// Directional light parameters.
#pragma once

#include "geometry/vec3.hpp"
#include "geometry/vec4.hpp"
#include "geometry/mat4x4.hpp"

namespace lili {

class DirectionalLight {
public:
	/// Create a directional light with defaults.
	DirectionalLight();
	/// Destroy the directional light.
	~DirectionalLight() = default;

	/// Set the light direction.
	void set_direction(const Vec3 &dir);
	/// Set the light color and intensity.
	void set_color(const Vec4 &rgba);
	/// Set the shadow distance for the light volume.
	void set_shadow_distance(float value);
	/// Set the orthographic half-size of the light volume.
	void set_orthogonal_half(float value);
	/// Recenter the light on a target point.
	void update_focus(const Vec3 &target_point);

	/// Get the light direction.
	Vec3 get_direction() const;
	/// Get the light color.
	Vec4 get_color() const;
	/// Get the light projection-view matrix.
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
