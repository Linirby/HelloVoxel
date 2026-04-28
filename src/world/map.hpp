#ifndef WORLD_MAP_HPP
# define WORLD_MAP_HPP

# include <unordered_map>

# include "world/chunk.hpp"
# include "math/vec3.hpp"

namespace lili {

struct RaycastResult {
	bool hit = false;
	int hit_x, hit_y, hit_z;
	int adjacent_x, adjacent_y, adjacent_z;
};

struct Map {
	~Map();

	std::unordered_map<uint64_t, Chunk> chunks;

	void set_block_global(uint8_t id, int x, int y, int z);
	uint8_t get_block_global(int x, int y, int z) const;
	uint64_t get_chunk_key(int x, int y, int z) const;

	RaycastResult raycast(
		const Vec3 &origin, const Vec3 &direction, float max_distance
	);
};

}  // namespace lili

#endif  // WORLD_MAP_HPP
