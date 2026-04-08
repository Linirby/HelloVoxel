#include <cmath>
#include "geometry/camera.hpp"
#include "math/utils.hpp"

namespace lili {

Camera::Camera() {
	update_vectors();
}

void Camera::process_mouse(float x_offset, float y_offset) {
	yaw += x_offset * mouse_sensitivity;
	pitch -= y_offset * mouse_sensitivity;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	update_vectors();
}

Mat4 Camera::get_view_matrix() const {
	return Mat4::look_at(position, position + front, { 0.0f, 1.0f, 0.0f });
}

void Camera::update_vectors() {
	Vec3 new_front;

	new_front.x = cos(deg_to_rad(pitch)) * cos(deg_to_rad(yaw));
	new_front.y = sin(deg_to_rad(pitch));
	new_front.z = cos(deg_to_rad(pitch)) * sin(deg_to_rad(yaw));

	front = new_front.normalized();
	right = front.cross({ 0.0f, 1.0f, 0.0f }).normalized();
	up = right.cross(front).normalized();
}

}  // namespace lili
