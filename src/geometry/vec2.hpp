/// 2D vector math type.
#pragma once

namespace lili {

struct Vec2 {
	float x, y;

	/// Assign from another vector.
	Vec2 operator=(const Vec2 &o);
	/// Subtract another vector.
	Vec2 operator-(const Vec2 &o) const;
	/// Subtract another vector in place.
	Vec2 operator-=(const Vec2 &o);
	/// Add another vector.
	Vec2 operator+(const Vec2 &o) const;
	/// Add another vector in place.
	Vec2 operator+=(const Vec2 &o);
	/// Multiply by a scalar.
	Vec2 operator*(const float scalar) const;
	/// Multiply by a scalar in place.
	Vec2 operator*=(const float scalar);

	/// Dot product with another vector.
	float dot(const Vec2 &o) const;
	/// 2D cross product (returns scalar).
	float cross(const Vec2 &o) const;
	/// Return a normalized copy of this vector.
	Vec2 normalized() const;
	/// Linearly interpolate toward another vector.
	Vec2 lerp(const Vec2 &o) const;
	/// Get the vector length.
	float length() const;
};

}  // namespace lili
