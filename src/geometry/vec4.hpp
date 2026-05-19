/// 4D vector math type.
#pragma once

namespace lili {

struct Vec4 {
	float x, y, z, w;

	/// Assign from another vector.
	Vec4 operator=(const Vec4 &o);
	/// Subtract another vector.
	Vec4 operator-(const Vec4 &o) const;
	/// Subtract another vector in place.
	Vec4 operator-=(const Vec4 &o);
	/// Add another vector.
	Vec4 operator+(const Vec4 &o) const;
	/// Add another vector in place.
	Vec4 operator+=(const Vec4 &o);
	/// Multiply by a scalar.
	Vec4 operator*(const float scalar) const;
	/// Multiply by a scalar in place.
	Vec4 operator*=(const float scalar);

	/// Dot product with another vector.
	float dot(const Vec4 &o) const;
	/// Cross product with another vector (XYZ components).
	Vec4 cross(const Vec4 &o) const;
	/// Return a normalized copy of this vector.
	Vec4 normalized() const;
	/// Get the vector length.
	float length() const;
};

}  // namespace lili
