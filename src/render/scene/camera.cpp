#include <cmath>
#include "render/scene/camera.hpp"
#include "geometry/utils.hpp"

namespace lili {

bool Frustum::contains(const AABB &box) const {
	for (const auto& plane : planes) {
		Vec3 positive = {
			plane.x >= 0 ? box.max.x : box.min.x,
			plane.y >= 0 ? box.max.y : box.min.y,
			plane.z >= 0 ? box.max.z : box.min.z,
		};
		if (plane.x*positive.x + plane.y*positive.y + plane.z*positive.z + plane.w < 0)
			return false;
	}
	return true;
}

Camera::Camera() {
	this->yaw = -90.0f;
	this->pitch = 0.0f;
	update_vectors();
	this->fov_y = 70.0f;
}

void Camera::set_rotation(float new_yaw, float new_pitch) {
	yaw = new_yaw;
	pitch = new_pitch;
	update_vectors();
}

void Camera::set_fov(float degree) {
	fov_y = degree;
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

static Frustum extract_frustum(const Mat4 &view_projection) {
	Frustum frustum;

	frustum.planes[0] = (Vec4){
		view_projection.m[3] + view_projection.m[0],
		view_projection.m[7] + view_projection.m[4],
		view_projection.m[11] + view_projection.m[8],
		view_projection.m[15] + view_projection.m[12]
	}.normalized();
	frustum.planes[1] = (Vec4){
		view_projection.m[3] - view_projection.m[0],
		view_projection.m[7] - view_projection.m[4],
		view_projection.m[11] - view_projection.m[8],
		view_projection.m[15] - view_projection.m[12]
	}.normalized();
	frustum.planes[2] = (Vec4){
		view_projection.m[3] + view_projection.m[1],
		view_projection.m[7] + view_projection.m[5],
		view_projection.m[11] + view_projection.m[9],
		view_projection.m[15] + view_projection.m[13]
	}.normalized();
	frustum.planes[3] = (Vec4){
		view_projection.m[3] - view_projection.m[1],
		view_projection.m[7] - view_projection.m[5],
		view_projection.m[11] - view_projection.m[9],
		view_projection.m[15] - view_projection.m[13]
	}.normalized();
	frustum.planes[4] = (Vec4){
		view_projection.m[3] + view_projection.m[2],
		view_projection.m[7] + view_projection.m[6],
		view_projection.m[11] + view_projection.m[10],
		view_projection.m[15] + view_projection.m[14]
	}.normalized();
	frustum.planes[5] = (Vec4){
		view_projection.m[3] - view_projection.m[2],
		view_projection.m[7] - view_projection.m[6],
		view_projection.m[11] - view_projection.m[10],
		view_projection.m[15] - view_projection.m[14]
	}.normalized();

	return frustum;
}

Mat4 Camera::get_projection(float aspect_ratio) const {
	return Mat4::perspective(fov_y, aspect_ratio, near_dist, far_dist);
}

Frustum Camera::get_frustum(float aspect_ratio) const {
	Mat4 vp = get_projection(aspect_ratio) * get_view_matrix();
	return extract_frustum(vp);
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
