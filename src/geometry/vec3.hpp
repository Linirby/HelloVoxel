/// 3D vector math type.
#pragma once

namespace lili {

struct Vec3 {
	float x, y, z;

	/// Assign from another vector.
	Vec3 operator=(const Vec3 &o);
	/// Subtract another vector.
	Vec3 operator-(const Vec3 &o) const;
	/// Subtract another vector in place.
	Vec3 operator-=(const Vec3 &o);
	/// Add another vector.
	Vec3 operator+(const Vec3 &o) const;
	/// Add another vector in place.
	Vec3 operator+=(const Vec3 &o);
	/// Multiply by a scalar.
	Vec3 operator*(const float scalar) const;
	/// Multiply by a scalar in place.
	Vec3 operator*=(const float scalar);

	/// Dot product with another vector.
	float dot(const Vec3 &o) const;
	/// Cross product with another vector.
	Vec3 cross(const Vec3 &o) const;
	/// Return a normalized copy of this vector.
	Vec3 normalized() const;
	/// Linearly interpolate toward another vector.
	Vec3 lerp(const Vec3 &o, float t) const;
	/// Get the vector length.
	float length() const;
};

}  // namespace lili
