/// 4x4 matrix math type.
#pragma once

#include "geometry/vec3.hpp"

namespace lili {

struct Mat4 {
	float m[16] = { 0.0f };

	/// Multiply by another matrix.
	Mat4 operator*(const Mat4 &other) const;

	/// Create an identity matrix.
	static Mat4 identity();
	/// Create a look-at view matrix.
	static Mat4 look_at(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
	/// Create a perspective projection matrix.
	static Mat4 perspective(
		float fovy_rad, float aspect, float near, float far
	);
	/// Create an orthographic projection matrix.
	static Mat4 orthographic(
		float left, float right, float bottom, float top, float near, float far
	);
	/// Create a translation matrix.
	static Mat4 translate(const Vec3 &pos);
	/// Create a rotation matrix around X axis.
	static Mat4 rotation_x(float radian);
	/// Create a rotation matrix around Y axis.
	static Mat4 rotation_y(float radian);
	/// Create a rotation matrix around Z axis.
	static Mat4 rotation_z(float radian);
	/// Create a rotation matrix from XYZ Euler angles.
	static Mat4 rotation_xyz(Vec3 radians);
	/// Create a scale matrix.
	static Mat4 scale(const Vec3 &v);
};

}  // namespace lili
