#ifndef MAT4X4_HPP
# define MAT4X4_HPP

# include <cmath>
# include "math/vec3.hpp"

namespace lili {

struct Mat4 {
	float m[16] = { 0.0f };

	Mat4 operator*(const Mat4 &other) const {
		Mat4 result;
		
		for (int col = 0; col < 4; ++col) {
			for (int row = 0; row < 4; ++row) {
				result.m[col * 4 + row] = (
					m[0 * 4 + row] * other.m[col * 4 + 0] +
					m[1 * 4 + row] * other.m[col * 4 + 1] +
					m[2 * 4 + row] * other.m[col * 4 + 2] +
					m[3 * 4 + row] * other.m[col * 4 + 3]
				);
			}
		}
		return result;
	}

	static Mat4 identity() {
		Mat4 result;

		result.m[0] = 1.0f;
		result.m[5] = 1.0f;
		result.m[10] = 1.0f;
		result.m[15] = 1.0f;
		return result;
	}

	static Mat4 look_at(const Vec3 &eye, const Vec3 &center, const Vec3 up) {
		Vec3 f = (center - eye).normalized();
		Vec3 s = f.cross(up).normalized();
		Vec3 u = s.cross(f);
		Mat4 result;
		// X axis
		result.m[0] = s.x;
		result.m[1] = u.x;
		result.m[2] = -f.x;
		// Y axis
		result.m[4] = s.y;
		result.m[5] = u.y;
		result.m[6] = -f.y;
		// Z axis
		result.m[8] = s.z;
		result.m[9] = u.z;
		result.m[10] = -f.z;
		// Camera translation
		result.m[12] = -s.dot(eye);
		result.m[13] = -u.dot(eye);
		result.m[14] = f.dot(eye);
		result.m[15] = 1.0f;
		return result;
	}

	static Mat4 perspective(
		float fovy_rad, float aspect, float near, float far
	) {
		Mat4 result;

		float tan_half_fovy = std::tan(fovy_rad * 0.5f);
		result.m[0] = 1.0f / (aspect * tan_half_fovy);
		result.m[5] = 1.0f / tan_half_fovy;
		result.m[10] = -far / (far - near);
		result.m[11] = -1.0f;
		result.m[14] = -(far * near) / (far - near);
		return result;
	}
};

} // namespace lili

#endif
