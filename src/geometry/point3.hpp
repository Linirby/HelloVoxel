/// 3D point type.
#pragma once

namespace lili {

struct Point3 {
	int x, y, z;

	/// Assign from another point.
	Point3 operator=(const Point3 &o);
};

}  // namespace lili
