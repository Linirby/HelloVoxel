#include "geometry/point3.hpp"
namespace lili {

Point3 Point3::operator=(const Point3 &o) {
	x = o.x;
	y = o.y;
	z = o.z;
	return *this;
}

}  // namespace lili
