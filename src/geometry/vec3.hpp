#pragma once

#include <cstdint>
#include <bit>

namespace lili {

inline float rsqrt(float number) {
	float x2 = number * 0.5f;
	uint32_t i = std::bit_cast<uint32_t>(number);
	i = 0x5f3759df - (i >> 1);
	float y = std::bit_cast<float>(i);
	y = y * (1.5f - (x2 * y * y));
	return y;
}

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