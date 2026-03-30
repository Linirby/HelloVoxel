#ifndef MAT4X4_HPP
# define MAT4X4_HPP

# include <cmath>
# include "math/vec3.hpp"

namespace lili {

struct mat4 {
	float m[16] = { 0.0f };

	mat4 operator*(const mat4 &other) const {
		mat4 result;
		
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

	static mat4 identity() {
		mat4 result;

		result.m[0] = 1.0f;
		result.m[5] = 1.0f;
		result.m[10] = 1.0f;
		result.m[15] = 1.0f;
		return result;
	}

	static mat4 look_at(const vec3 &eye, const vec3 &center, const vec3 up) {
		vec3 f = (center - eye).normalized();
		vec3 s = f.cross(up).normalized();
		vec3 u = s.cross(f);
		mat4 result;
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

	static mat4 perspective(
		float fovy_rad, float aspect, float near, float far
	) {
		mat4 result;

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
