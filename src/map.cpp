#include "map.hpp"

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

}  // namespace lili
