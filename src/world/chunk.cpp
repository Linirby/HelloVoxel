#include "world/chunk.hpp"

namespace lili {

void Chunk::set_block(uint8_t id, int x, int y, int z) {
	blocks[x + y * SIZE + z * SIZE * SIZE] = id;
}

uint8_t Chunk::get_block(int x, int y, int z) const {
	return blocks[x + y * SIZE + z * SIZE * SIZE];
}

AABB Chunk::get_bounds(int chunk_x, int chunk_y, int chunk_z) const {
	Vec3 world_pos = {
		(float)chunk_x * SIZE, (float)chunk_y * SIZE, (float)chunk_z * SIZE
	};
	return { world_pos, world_pos + (Vec3){ SIZE, SIZE, SIZE } };
}

}  // namespace lili
