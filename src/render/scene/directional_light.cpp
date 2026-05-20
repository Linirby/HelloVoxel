#include "render/scene/directional_light.hpp"

namespace lili {

DirectionalLight::DirectionalLight() {
	direction = Vec3{ -0.5f, -1.0f, -0.3f }.normalized();
	color = { 1.0f, 1.0f, 0.9f, 0.75f };
	shadow_distance = 100.0f;
	orthogonal_half = 50.0f;
	update_focus({ 0.0f, 0.0f, 0.0f });
}

void DirectionalLight::set_direction(const Vec3 &dir) {
	direction = dir.normalized();
}

void DirectionalLight::set_color(const Vec4 &rgba) {
	color = rgba;
}

void DirectionalLight::set_shadow_distance(float value) {
	shadow_distance = value;
}

void DirectionalLight::set_orthogonal_half(float value) {
	orthogonal_half = value;
}

void DirectionalLight::update_focus(const Vec3 &target_point) {
	focus_point = target_point;
	position = focus_point - direction * shadow_distance;

	Vec3 up_vector = (direction.y > 0.0f) ?
		Vec3{ 0.0f, -1.0f, 0.0f } : Vec3{ 0.0f, 1.0f, 0.0f };

	view = Mat4::look_at(position, focus_point, up_vector);
	projection = Mat4::orthographic(
		-orthogonal_half, orthogonal_half,
		orthogonal_half, -orthogonal_half,
		0.1f, shadow_distance * 2.0f
	);
	projection_view = projection * view;
}

Vec3 DirectionalLight::get_direction() const {
	return direction;
}

Vec4 DirectionalLight::get_color() const {
	return color;
}

Mat4 DirectionalLight::get_projection_view() const {
	return projection_view;
}

}  // namespace lili
