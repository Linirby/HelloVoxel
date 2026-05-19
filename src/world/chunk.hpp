/// Voxel chunk data structure.
#pragma once

#include <cstdint>

namespace lili {

struct Chunk {
	static constexpr int SIZE = 16;
	uint8_t blocks[SIZE * SIZE * SIZE] = { 0 };

	/// Set a block in local chunk coordinates.
	void set_block(uint8_t id, int x, int y, int z);
	/// Get a block in local chunk coordinates.
	uint8_t get_block(int x, int y, int z) const;
};

}  // namespace lili
