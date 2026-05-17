#pragma once

namespace lili {

struct Vec2 {
	float x, y;

	Vec2 operator=(const Vec2 &o);
	Vec2 operator-(const Vec2 &o) const;
	Vec2 operator-=(const Vec2 &o);
	Vec2 operator+(const Vec2 &o) const;
	Vec2 operator+=(const Vec2 &o);
	Vec2 operator*(const float scalar) const;
	Vec2 operator*=(const float scalar);

	float dot(const Vec2 &o) const;
	float cross(const Vec2 &o) const;
	Vec2 normalized() const;
	Vec2 lerp(const Vec2 &o) const;
	float length() const;
};

}  // namespace lili
