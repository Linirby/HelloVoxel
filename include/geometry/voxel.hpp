#ifndef GEOMETRY_VOXEL_HPP
# define GEOMETRY_VOXEL_HPP

# include "math/vec3.hpp"
# include "geometry/core.hpp"

namespace lili {

struct Voxel {
	Vec3 position;
	Vec3 size;
	Vertex vertices[8] = {
		{
			position.x,
			-position.y,
			position.z,
			1.0f, 0.0f, 0.0f, 1.0f
		},
		{
			position.x + size.x,
			-position.y,
			position.z,
			0.0f, 1.0f, 0.0f, 1.0f
		},
		{
			position.x + size.x,
			-(position.y + size.y),
			position.z,
			0.0f, 0.0f, 1.0f, 1.0f
		},
		{
			position.x,
			-(position.y + size.x),
			position.z,
			1.0f, 1.0f, 0.0f, 1.0f
		},
		{
			position.x,
			-position.y,
			position.z - size.z,
			1.0f, 0.0f, 1.0f, 1.0f
		},
		{
			position.x + size.x,
			-position.y,
			position.z - size.z,
			0.0f, 1.0f, 1.0f, 1.0f
		},
		{
			position.x + size.x,
			-(position.y + size.y),
			position.z - size.z,
			1.0f, 1.0f, 1.0f, 1.0f
		},
		{
			position.x,
			-(position.y + size.x),
			position.z - size.z,
			0.0f, 0.0f, 0.0f, 1.0f
		}
	};
	uint16_t indices[36] = {
		0, 1, 2,  2, 3, 0,
		1, 5, 6,  6, 2, 1,
		5, 4, 7,  7, 6, 5,
		4, 0, 3,  3, 7, 4,
		4, 5, 1,  1, 0, 4,
		3, 2, 6,  6, 7, 3
	};
};

}  // namespace lili

#endif  // GEOMETRY_VOXEL_HPP
