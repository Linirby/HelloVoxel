#pragma once

namespace lili {

struct Vec4 {
	float x, y, z, w;

	Vec4 operator=(const Vec4 &o);
	Vec4 operator-(const Vec4 &o) const;
	Vec4 operator-=(const Vec4 &o);
	Vec4 operator+(const Vec4 &o) const;
	Vec4 operator+=(const Vec4 &o);
	Vec4 operator*(const float scalar) const;

	float dot(const Vec4 &o) const;
	Vec4 cross(const Vec4 &o) const;
	Vec4 normalized() const;
	float length() const;
};

}  // namespace lili
