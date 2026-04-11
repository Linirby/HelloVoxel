#include <cmath>
#include "math/mat4x4.hpp"

namespace lili {

Mat4 Mat4::operator*(const Mat4 &other) const {
	Mat4 result;

	for (int col = 0; col < 4; ++col) {
		for (int row = 0; row < 4; ++row) {
			result.m[col * 4 + row] = (
				m[0 * 4 + row] * other.m[col * 4 + 0] +
				m[1 * 4 + row] * other.m[col * 4 + 1] +
				m[2 * 4 + row] * other.m[col * 4 + 2] +
				m[3 * 4 + row] * other.m[col * 4 + 3]
			);
		}
	}
	return result;
}

Mat4 Mat4::identity() {
	Mat4 result;

	result.m[0] = 1.0f;
	result.m[5] = 1.0f;
	result.m[10] = 1.0f;
	result.m[15] = 1.0f;
	return result;
}

Mat4 Mat4::look_at(const Vec3 &eye, const Vec3 &center, const Vec3 &up) {
	Vec3 f = (center - eye).normalized();
	Vec3 s = f.cross(up).normalized();
	Vec3 u = s.cross(f);
	Mat4 result;

	// X axis
	result.m[0] = s.x;
	result.m[1] = u.x;
	result.m[2] = -f.x;
	// Y axis
	result.m[4] = s.y;
	result.m[5] = u.y;
	result.m[6] = -f.y;
	// Z axis
	result.m[8] = s.z;
	result.m[9] = u.z;
	result.m[10] = -f.z;
	// Camera translation
	result.m[12] = -s.dot(eye);
	result.m[13] = -u.dot(eye);
	result.m[14] = f.dot(eye);
	result.m[15] = 1.0f;
	return result;
}

Mat4 Mat4::perspective(
	float fovy_rad, float aspect, float near, float far
) {
	Mat4 result;

	float tan_half_fovy = std::tan(fovy_rad * 0.5f);
	result.m[0] = 1.0f / (aspect * tan_half_fovy);
	result.m[5] = 1.0f / tan_half_fovy;
	result.m[10] = -far / (far - near);
	result.m[11] = -1.0f;
	result.m[14] = -(far * near) / (far - near);
	return result;
}

Mat4 Mat4::orthographic(
	float left, float right, float bottom, float top, float near, float far
) {
	Mat4 result;

	result.m[0] = 2.0f / (right - left);
	result.m[5] = 2.0f / (top - bottom);
	result.m[10] = -2.0f / (far - near);
	result.m[12] = -(right + left) / (right - left);
	result.m[13] = -(top + bottom) / (top - bottom);
	result.m[14] = -(far + near) / (far - near);
	result.m[15] = 1.0f;
	return result;
}

Mat4 Mat4::translate(const Vec3 &pos) {
	Mat4 result = Mat4::identity();

	result.m[12] = pos.x;
	result.m[13] = pos.y;
	result.m[14] = pos.z;
	return result;
}

Mat4 Mat4::rotation_x(float radians) {
	Mat4 result;
	float c = std::cos(radians);
	float s = std::sin(radians);

	result = Mat4::identity();
	result.m[5] = c;
	result.m[6] = s;
	result.m[9] = -s;
	result.m[10] = c;
	return result;
}

Mat4 Mat4::rotation_y(float radians) {
	Mat4 result;
	float c = std::cos(radians);
	float s = std::sin(radians);

	result = Mat4::identity();
	result.m[0] = c;
	result.m[2] = -s;
	result.m[8] = s;
	result.m[10] = c;
	return result;
}

Mat4 Mat4::rotation_z(float radians) {
	Mat4 result;
	float c = std::cos(radians);
	float s = std::sin(radians);

	result = Mat4::identity();
	result.m[0] = c;
	result.m[1] = s;
	result.m[4] = -s;
	result.m[5] = c;
	return result;
}

}
