#include "math/vec3.hpp"

namespace lili {

Vec3 Vec3::operator-(const Vec3 &other) const {
	return { x - other.x, y - other.y, z - other.z };
}

Vec3 Vec3::operator+(const Vec3 &other) const {
	return { x + other.x, y + other.y, z + other.z };
}

Vec3 Vec3::operator*(const float scalar) const {
	return { x * scalar, y * scalar, z * scalar };
}

float Vec3::dot(const Vec3 &other) const {
	return x * other.x + y * other.y + z * other.z;
}

Vec3 Vec3::cross(const Vec3 &other) const {
	return {
		y * other.z - z * other.y,
		z * other.x - x * other.z,
		x * other.y - y * other.x
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
