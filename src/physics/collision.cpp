#include "physics/collision.hpp"

#include <cmath>
#include "physics/world_interface.hpp"

namespace lili {

bool AABB::intersect(const AABB &other) const {
	return (
		(min.x <= other.max.x && max.x >= other.min.x) &&
		(min.y <= other.max.y && max.y >= other.min.y) &&
		(min.z <= other.max.z && max.z >= other.min.z)
	);
}

bool check_voxel_collision(const AABB &box, const IVoxelWorld &world) {
	float eps = 0.01f;

	int x = std::floor(box.min.x + eps);
	for (; x <= std::floor(box.max.x - eps); ++x) {

		int y = std::floor(box.min.y + eps);
		for (; y <= std::floor(box.max.y - eps); ++y) {

			int z = std::floor(box.min.z + eps);
			for (; z <= std::floor(box.max.z - eps); ++z) {
				if (world.is_solid_at(x, y, z)) return true;
			}
		}
	}
	return false;
}

RaycastResult raycast_voxel(
	const Vec3 &origin,
	const Vec3 &dir,
	float max_dist,
	const IVoxelWorld &world
) {
	RaycastResult result;

	int x = std::floor(origin.x);
	int y = std::floor(origin.y);
	int z = std::floor(origin.z);

	int step_x = (dir.x > 0) - (dir.x < 0);
	int step_y = (dir.y > 0) - (dir.y < 0);
	int step_z = (dir.z > 0) - (dir.z < 0);

	Vec3 t_delta{
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity(),
		std::numeric_limits<float>::infinity()
	};
	if (step_x != 0) t_delta.x = std::abs(1.0f / dir.x);
	if (step_y != 0) t_delta.y = std::abs(1.0f / dir.y);
	if (step_z != 0) t_delta.z = std::abs(1.0f / dir.z);

	Vec3 t_max{
		(origin.x - x) * t_delta.x,
		(origin.y - y) * t_delta.y,
		(origin.z - z) * t_delta.z
	};
	if (step_x > 0) t_max.x = (x + 1.0f - origin.x) * t_delta.x;
	if (step_y > 0) t_max.y = (y + 1.0f - origin.y) * t_delta.y;
	if (step_z > 0) t_max.z = (z + 1.0f - origin.z) * t_delta.z;

	float distance = 0.0f;
	int last_step = 0;  // X: 0 | Y: 1 | Z: 2

	while (distance <= max_dist) {
		if (world.is_solid_at(x, y, z)) {
			result.hit = true;

			result.hit_x = x;
			result.hit_y = y;
			result.hit_z = z;

			result.adj_x = x;
			result.adj_y = y;
			result.adj_z = z;

			if (last_step == 0) result.adj_x -= step_x;
			else if (last_step == 1) result.adj_y -= step_y;
			else if (last_step == 2) result.adj_z -= step_z;

			return result;
		}
		if (t_max.x < t_max.y) {
			if (t_max.x < t_max.z) {
				x += step_x;
				distance = t_max.x;
				t_max.x += t_delta.x;
				last_step = 0;
			} else {
				z += step_z;
				distance = t_max.z;
				t_max.z += t_delta.z;
				last_step = 2;
			}
		} else {
			if (t_max.y < t_max.z) {
				y += step_y;
				distance = t_max.y;
				t_max.y += t_delta.y;
				last_step = 1;
			} else {
				z += step_z;
				distance = t_max.z;
				t_max.z += t_delta.z;
				last_step = 2;
			}
		}
	}
	return result;
}

}  // namespace lili

