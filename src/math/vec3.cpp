#include "math/vec3.hpp"

namespace lili {

Vec3 Vec3::operator=(const Vec3 &o) {
	x = o.x;
	y = o.y;
	z = o.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3 &o) const {
	return { x - o.x, y - o.y, z - o.z };
}

Vec3 Vec3::operator-=(const Vec3 &o) {
	x = x - o.x;
	y = y - o.y;
	z = z - o.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 &o) const {
	return { x + o.x, y + o.y, z + o.z };
}

Vec3 Vec3::operator+=(const Vec3 &o) {
	x = x + o.x;
	y = y + o.y;
	z = z + o.z;
	return *this;
}

Vec3 Vec3::operator*(const float scalar) const {
	return { x * scalar, y * scalar, z * scalar };
}

float Vec3::dot(const Vec3 &o) const {
	return x * o.x + y * o.y + z * o.z;
}

Vec3 Vec3::cross(const Vec3 &o) const {
	return {
		y * o.z - z * o.y,
		z * o.x - x * o.z,
		x * o.y - y * o.x
	};
}

Vec3 Vec3::normalized() const {
	float sq_len = x*x + y*y + z*z;
	if (sq_len == 0.0f) {
		return { 0.0f, 0.0f, 0.0f };
	}
	return *this * rsqrt(sq_len);
}

}  // namespace lili
