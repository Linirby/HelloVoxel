#pragma once

#include <vector>
#include <cstdint>

namespace lili {

struct Vertex {
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
};

MeshData create_unit_quad();

}  // namespace lili