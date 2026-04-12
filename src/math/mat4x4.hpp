#ifndef MAT4X4_HPP
# define MAT4X4_HPP

# include "math/vec3.hpp"

namespace lili {

struct Mat4 {
	float m[16] = { 0.0f };

	Mat4 operator*(const Mat4 &other) const;

	static Mat4 identity();
	static Mat4 look_at(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
	static Mat4 perspective(
		float fovy_rad, float aspect, float near, float far
	);
	static Mat4 orthographic(
		float left, float right, float bottom, float top, float near, float far
	);
	static Mat4 translate(const Vec3 &pos);
	static Mat4 rotation_x(float radians);
	static Mat4 rotation_y(float radians);
	static Mat4 rotation_z(float radians);
	static Mat4 scale(const Vec3 &v);
};

} // namespace lili

#endif
