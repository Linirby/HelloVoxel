#include "map.hpp"
#include "math/utils.hpp"

namespace lili {

Map::~Map() {
	chunks.clear();
}

void Map::set_block_global(uint8_t id, int x, int y, int z) {
	int chunk_x = x >> 4;
	int chunk_y = y >> 4;
	int chunk_z = z >> 4;

	int local_x = x & 15;
	int local_y = y & 15;
	int local_z = z & 15;
	
	uint64_t key = get_chunk_key(chunk_x, chunk_y, chunk_z);
	chunks[key].set_block(id, local_x, local_y, local_z);
}

uint8_t Map::get_block_global(int x, int y, int z) const {
	int chunk_x = x >> 4;
	int chunk_y = y >> 4;
	int chunk_z = z >> 4;

	uint64_t key = get_chunk_key(chunk_x, chunk_y, chunk_z);
	auto it = chunks.find(key);

	if (it == chunks.end()) return 0;

	int local_x = x & 15;
	int local_y = y & 15;
	int local_z = z & 15;
	return it->second.get_block(local_x, local_y, local_z);
}

uint64_t Map::get_chunk_key(int x, int y, int z) const {
	return (
		(static_cast<uint64_t>(static_cast<uint16_t>(x)) << 32) |
		(static_cast<uint64_t>(static_cast<uint16_t>(y)) << 16) |
		(static_cast<uint64_t>(static_cast<uint16_t>(z)))
	);
}

RaycastResult Map::raycast(
	const Vec3 &origin, const Vec3 &direction, float max_distance
) {
	RaycastResult result;

	int x = lili::floor(origin.x);
	int y = lili::floor(origin.y);
	int z = lili::floor(origin.z);

	int step_x, step_y, step_z = 0;
	if (direction.x > 0) step_x = 1;
	else if (direction.x < 0) step_x = -1;
	if (direction.y > 0) step_y = 1;
	else if (direction.y < 0) step_y = -1;
	if (direction.z > 0) step_z = 1;
	else if (direction.z < 0) step_z = -1;

	Vec3 t_delta{ lili::F_INFINITY, lili::F_INFINITY, lili::F_INFINITY };
	if (step_x != 0) t_delta.x = lili::abs(1.0f / direction.x);
	if (step_y != 0) t_delta.y = lili::abs(1.0f / direction.y);
	if (step_z != 0) t_delta.z = lili::abs(1.0f / direction.z);

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

	while (distance <= max_distance) {
		if (get_block_global(x, y, z)) {
			result.hit = true;

			result.hit_x = x;
			result.hit_y = y;
			result.hit_z = z;

			result.adjacent_x = x;
			result.adjacent_y = y;
			result.adjacent_z = z;

			if (last_step == 0) result.adjacent_x -= step_x;
			else if (last_step == 1) result.adjacent_y -= step_y;
			else if (last_step == 2) result.adjacent_z -= step_z;

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
