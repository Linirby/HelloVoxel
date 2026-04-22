#ifndef MATH_UTILS_HPP
# define MATH_UTILS_HPP

# include <climits>
# include <limits>

namespace lili {

constexpr float PI = 3.14159265359f;
constexpr float F_INFINITY = std::numeric_limits<float>::infinity();

inline float deg_to_rad(float degrees) {
	return degrees * PI / 180.0f;
}

inline double floor(double num) {
	if (num >= LLONG_MAX || num <= LLONG_MIN || num != num) return num;

	long long n = (long long)num;
	double d = (double)n;
	if (d == num || num >= 0) return d;
	else return d - 1;
}

inline float abs(float n) {
	return n < 0 ? -n : n;
}

inline int abs(int n) {
	return n < 0 ? -n : n;
}

}  // namespace lili

#endif
