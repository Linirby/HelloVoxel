#ifndef GEOMETRY_VOXEL_HPP
# define GEOMETRY_VOXEL_HPP

# include "math/vec3.hpp"
# include "geometry/core.hpp"

namespace lili {

struct Voxel {
	Vec3 pos;
	Vec3 size;
	Vertex vertices[24] = {
		// TOP FACE
		{ pos.x, pos.y, pos.z - size.z, 0.0f, 0.0f },
		{ pos.x + size.x, pos.y, pos.z - size.z, 1.0f, 0.0f },
		{ pos.x + size.x, pos.y, pos.z, 1.0f, 1.0f },
		{ pos.x, pos.y, pos.z, 0.0f, 1.0f },
		// BOTTOM FACE
		{ pos.x, pos.y - size.y, pos.z, 0.0f, 0.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z, 1.0f, 0.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z - size.z, 1.0f, 1.0f },
		{ pos.x, pos.y - size.y, pos.z - size.z, 0.0f, 1.0f },
		// RIGHT FACE
		{ pos.x + size.x, pos.y, pos.z, 0.0f, 0.0f },
		{ pos.x + size.x, pos.y, pos.z - size.z, 1.0f, 0.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z - size.z, 1.0f, 1.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z, 0.0f, 1.0f },
		// LEFT FACE
		{ pos.x, pos.y, pos.z - size.z, 0.0f, 0.0f },
		{ pos.x, pos.y, pos.z, 1.0f, 0.0f },
		{ pos.x, pos.y - size.y, pos.z, 1.0f, 1.0f },
		{ pos.x, pos.y - size.y, pos.z - size.z, 0.0f, 1.0f },
		// FRONT FACE
		{ pos.x, pos.y, pos.z, 0.0f, 0.0f },
		{ pos.x + size.x, pos.y, pos.z, 1.0f, 0.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z, 1.0f, 1.0f },
		{ pos.x, pos.y - size.x, pos.z, 0.0f, 1.0f },
		// BACK FACE
		{ pos.x + size.x, pos.y, pos.z - size.z, 0.0f, 0.0f },
		{ pos.x, pos.y, pos.z - size.z, 1.0f, 0.0f },
		{ pos.x, pos.y - size.y, pos.z - size.z, 1.0f, 1.0f },
		{ pos.x + size.x, pos.y - size.x, pos.z - size.z, 0.0f, 1.0f },
	};
	uint16_t indices[36] = {
		 4,  5,  6,   6,  7,  4,  // Top
		 8,  9, 10,  10, 11,  8,  // Bottom
		12, 13, 14,  14, 15, 12,  // Right
		20, 21, 22,  22, 23, 20,  // Left
		 0,  1,  2,   2,  3,  0,  // Front
		16, 17, 18,  18, 19, 16,  // Back
	};
};

struct AtlasVoxel {
	Vec3 pos;
	Vec3 size;
	Vertex vertices[24] = {
		// FRONT FACE
		{ pos.x, pos.y, pos.z, 0.0f, 0.0f },
		{ pos.x + size.x, pos.y, pos.z, 0.25f, 0.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z, 0.25f, 0.5f },
		{ pos.x, pos.y - size.x, pos.z, 0.0f, 0.5f },
		// TOP FACE
		{ pos.x, pos.y, pos.z - size.z, 0.0f, 0.5f },
		{ pos.x + size.x, pos.y, pos.z - size.z, 0.25f, 0.5f },
		{ pos.x + size.x, pos.y, pos.z, 0.25f, 1.0f },
		{ pos.x, pos.y, pos.z, 0.0f, 1.0f },
		// BOTTOM FACE
		{ pos.x, pos.y - size.y, pos.z, 0.25f, 0.5f },
		{ pos.x + size.x, pos.y - size.y, pos.z, 0.5f, 0.5f },
		{ pos.x + size.x, pos.y - size.y, pos.z - size.z, 0.5f, 1.0f },
		{ pos.x, pos.y - size.y, pos.z - size.z, 0.25f, 1.0f },
		// RIGHT FACE
		{ pos.x + size.x, pos.y, pos.z, 0.25f, 0.0f },
		{ pos.x + size.x, pos.y, pos.z - size.z, 0.5f, 0.0f },
		{ pos.x + size.x, pos.y - size.y, pos.z - size.z, 0.5f, 0.5f },
		{ pos.x + size.x, pos.y - size.y, pos.z, 0.25f, 0.5f },
		// BACK FACE
		{ pos.x + size.x, pos.y, pos.z - size.z, 0.5f, 0.0f },
		{ pos.x, pos.y, pos.z - size.z, 0.75f, 0.0f },
		{ pos.x, pos.y - size.y, pos.z - size.z, 0.75f, 0.5f },
		{ pos.x + size.x, pos.y - size.x, pos.z - size.z, 0.5f, 0.5f },
		// LEFT FACE
		{ pos.x, pos.y, pos.z - size.z, 0.75f, 0.0f },
		{ pos.x, pos.y, pos.z, 1.0f, 0.0f },
		{ pos.x, pos.y - size.y, pos.z, 1.0f, 0.5f },
		{ pos.x, pos.y - size.y, pos.z - size.z, 0.75f, 0.5f },
	};
	uint16_t indices[36] = {
		 0,  1,  2,   2,  3,  0,  // Front
		 4,  5,  6,   6,  7,  4,  // Top
		 8,  9, 10,  10, 11,  8,  // Bottom
		12, 13, 14,  14, 15, 12,  // Right
		16, 17, 18,  18, 19, 16,  // Back
		20, 21, 22,  22, 23, 20   // Left
	};
};

}  // namespace lili

#endif  // GEOMETRY_VOXEL_HPP
