#pragma once

namespace lili {

struct Vec3 {
	float x, y, z;

	Vec3 operator=(const Vec3 &o);
	Vec3 operator-(const Vec3 &o) const;
	Vec3 operator-=(const Vec3 &o);
	Vec3 operator+(const Vec3 &o) const;
	Vec3 operator+=(const Vec3 &o);
	Vec3 operator*(const float scalar) const;

	float dot(const Vec3 &o) const;
	Vec3 cross(const Vec3 &o) const;
	Vec3 normalized() const;
	float length() const;
};

}  // namespace lili
