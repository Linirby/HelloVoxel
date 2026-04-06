#include "geometry/chunk.hpp"

namespace lili {

void Chunk::set_block(uint8_t id, int x, int y, int z) {
	blocks[x + y * SIZE + z * SIZE * SIZE] = id;
}

uint8_t Chunk::get_block(int x, int y, int z) const {
	return blocks[x + y * SIZE + z * SIZE * SIZE];
}

}  // namespace lili
