#include "geometry/vec2.hpp"
#include <math.h>

namespace lili {

Vec2 Vec2::operator=(const Vec2 &o) {
	x = o.x;
	y = o.y;
	return *this;
}

Vec2 Vec2::operator-(const Vec2 &o) const {
	return { x - o.x, y - o.y };
}

Vec2 Vec2::operator-=(const Vec2 &o) {
	x = x - o.x;
	y = y - o.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2 &o) const {
	return { x + o.x, y + o.y };
}

Vec2 Vec2::operator+=(const Vec2 &o) {
	x = x + o.x;
	y = y + o.y;
	return *this;
}

Vec2 Vec2::operator*(const float scalar) const {
	return { x * scalar, y * scalar };
}

Vec2 Vec2::operator*=(const float scalar) {
	x = x * scalar;
	y = y * scalar;
	return *this;
}

float Vec2::dot(const Vec2 &o) const {
	return x * o.x + y * o.y;
}

float Vec2::cross(const Vec2 &o) const {
	return x * o.y - y * o.x;
}

Vec2 Vec2::normalized() const {
	float sq_len = x*x + y*y;
	if (sq_len == 0.0f) {
		return { 0.0f, 0.0f };
	}
	return *this * (1 / std::sqrt(sq_len));
}

float Vec2::length() const {
	return std::sqrt(x * x + y * y);
}

}  // namespace lili
