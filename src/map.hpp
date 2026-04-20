#ifndef MAP_HPP
# define MAP_HPP

# include <unordered_map>

# include "geometry/chunk.hpp"

namespace lili {

struct Map {
	~Map();

	std::unordered_map<uint64_t, Chunk> chunks;

	void set_block_global(uint8_t id, int x, int y, int z);
	uint8_t get_block_global(int x, int y, int z) const;
	uint64_t get_chunk_key(int x, int y, int z) const;
};

}  // namespace lili

#endif
