/// Voxel chunk data structure.
#pragma once

#include <cstdint>
#include "physics/collision.hpp"

namespace lili {

struct Chunk {
	static constexpr int SIZE = 16;
	uint8_t blocks[SIZE * SIZE * SIZE] = { 0 };

	/// Set a block in local chunk coordinates.
	void set_block(uint8_t id, int x, int y, int z);
	/// Get a block in local chunk coordinates.
	uint8_t get_block(int x, int y, int z) const;
	/// Return the box that represent the bound of the chunk.
	AABB get_bounds(int chunk_x, int chunk_y, int chunk_z) const;
};

}  // namespace lili
