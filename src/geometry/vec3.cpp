#include "geometry/vec3.hpp"
#include <math.h>

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

Vec3 Vec3::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;
	z = z * scalar;
	return *this;
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
	return *this * (1 / std::sqrt(sq_len));
}

Vec3 Vec3::lerp(const Vec3 &o, float t) const {
	return {
		std::lerp(x, o.x, t),
		std::lerp(y, o.y, t),
		std::lerp(z, o.z, t)
	};
}

float Vec3::length() const {
	return std::sqrt(x * x + y * y + z * z);
}

}  // namespace lili
