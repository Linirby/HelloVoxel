#pragma once

#include <cstdint>
#include "world/chunk.hpp"

namespace lili {

struct ChunkNeighborhood {
	const Chunk *chunks[3][3][3];
	
	uint16_t get_block(int x, int y, int z) const;
};

}  // namespace lili
