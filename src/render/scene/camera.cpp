#include <cmath>
#include "render/scene/camera.hpp"
#include "geometry/utils.hpp"

namespace lili {

Camera::Camera(float yaw, float pitch, float fov_y) {
	this->yaw = yaw;
	this->pitch = pitch;
	this->fov_y = fov_y;
	update_vectors();
}

void Camera::set_rotation(float new_yaw, float new_pitch) {
	yaw = new_yaw;
	pitch = new_pitch;
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
	return Mat4::look_at(position, position + front, up);
}

void Camera::update_vectors() {
	Vec3 new_front;

	new_front.x = std::cos(deg_to_rad(pitch)) * std::cos(deg_to_rad(yaw));
	new_front.y = std::sin(deg_to_rad(pitch));
	new_front.z = std::cos(deg_to_rad(pitch)) * std::sin(deg_to_rad(yaw));

	front = new_front.normalized();
	right = front.cross({ 0.0f, 1.0f, 0.0f }).normalized();
	up = right.cross(front).normalized();
}

}  // namespace lili
