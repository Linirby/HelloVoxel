#ifndef MESHING_PRIMITIVE_HPP
# define MESHING_PRIMITIVE_HPP

# include <vector>
# include <cstdint>

namespace lili {

struct Vertex {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

MeshData create_unit_quad();

}  // namespace lili

#endif  // MESHING_PRIMITIVE_HPP
