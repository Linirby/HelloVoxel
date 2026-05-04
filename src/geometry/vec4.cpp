#include "geometry/vec4.hpp"
#include <math.h>

namespace lili {

Vec4 Vec4::operator=(const Vec4 &o) {
	x = o.x;
	y = o.y;
	z = o.z;
	w = o.w;
	return *this;
}

Vec4 Vec4::operator-(const Vec4 &o) const {
	return { x - o.x, y - o.y, z - o.z, w - o.w };
}

Vec4 Vec4::operator-=(const Vec4 &o) {
	x = x - o.x;
	y = y - o.y;
	z = z - o.z;
	w = w - o.w;
	return *this;
}

Vec4 Vec4::operator+(const Vec4 &o) const {
	return { x + o.x, y + o.y, z + o.z, w + o.w };
}

Vec4 Vec4::operator+=(const Vec4 &o) {
	x = x + o.x;
	y = y + o.y;
	z = z + o.z;
	w = w + o.w;
	return *this;
}

Vec4 Vec4::operator*(const float scalar) const {
	return { x * scalar, y * scalar, z * scalar, w * scalar };
}

float Vec4::dot(const Vec4 &o) const {
	return x * o.x + y * o.y + z * o.z + w * o.w;
}

Vec4 Vec4::cross(const Vec4 &o) const {
	return {
		y * o.z - z * o.y,
		z * o.x - x * o.z,
		x * o.y - y * o.x,
		0.0f
	};
}

Vec4 Vec4::normalized() const {
	float sq_len = x*x + y*y + z*z + w*w;
	if (sq_len == 0.0f) {
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}
	return *this * rsqrt(sq_len);
}

float Vec4::length() const {
	return std::sqrt(x * x + y * y + z * z + w * w);
}

}  // namespace lili
