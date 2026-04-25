#ifndef GEOMETRY_CORE_HPP
# define GEOMETRY_CORE_HPP

namespace lili {

struct Vertex {
	float x, y, z;     // (location 0)
	float nx, ny, nz;  // (location 1)
	float u, v;        // (location 2)
};

}  // namespace lili

#endif  // GEOMETRY_CORE_HPP
