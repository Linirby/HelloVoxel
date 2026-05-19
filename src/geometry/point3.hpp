#pragma once

namespace lili {

struct Point3 {
	int x, y, z;

	Point3 operator=(const Point3 &o);
};

}  // namespace lili
