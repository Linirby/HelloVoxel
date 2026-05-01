#pragma once

#include "geometry/vec3.hpp"

namespace lili {

class IVoxelWorld;

struct AABB {
	Vec3 min;
	Vec3 max;

	bool intersect(const AABB &other) const;
};

struct RaycastResult {
	bool hit = false;
	int hit_x, hit_y, hit_z;
	int adj_x, adj_y, adj_z;
};

bool check_voxel_collision(const AABB &box, const IVoxelWorld &world);
RaycastResult raycast_voxel(
	const Vec3 &origin,
	const Vec3 &dir,
	float max_dist,
	const IVoxelWorld &world
);

}  // namespace lili
