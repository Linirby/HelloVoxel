#ifndef MATH_UTILS_HPP
# define MATH_UTILS_HPP

namespace lili {

constexpr float PI = 3.14159265359f;

inline float deg_to_rad(float degrees) {
	return degrees * PI / 180.0f;
}

}  // namespace lili

#endif
