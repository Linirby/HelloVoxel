#ifndef VEC3_HPP
# define VEC3_HPP

# include <cstdint>

namespace lili {

inline float rsqrt(float number) {
	float x2 = number * 0.5f;
	float y = number;
	uint32_t i = *(uint32_t *)&y;

	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (x2 * y * y));
	return y;
}

struct Vec3 {
	float x, y, z;

	Vec3 operator-(const Vec3 &o) const {
		return { x - o.x, y - o.y, z - o.z };
	}

	Vec3 operator+(const Vec3 &o) const {
		return { x + o.x, y + o.y, z + o.z };
	}

	Vec3 operator*(const float scalar) const {
		return { x * scalar, y * scalar, z * scalar };
	}

	float dot(const Vec3 &o) const {
		return x * o.x + y * o.y + z * o.z;
	}

	Vec3 cross(const Vec3 &o) const {
		return {
			y * o.z - z * o.y,
			z * o.x - x * o.z,
			x * o.y - y * o.x
		};
	}

	Vec3 normalized() const {
		float sq_len = x*x + y*y + z*z;
		if (sq_len == 0.0f) {
			return { 0.0f, 0.0f, 0.0f };
		}
		return *this * rsqrt(sq_len);
	}
};

} // namespace lili

#endif
