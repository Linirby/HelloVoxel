#ifndef MAP_HPP
# define MAP_HPP

# include <map>

# include "geometry/chunk.hpp"

namespace lili {

class Map {
public:
	~Map();

	std::map<int, Chunk> chunks;

	void set_block_global(uint8_t id, int x, int y, int z);
	uint8_t get_block_global(int x, int y, int z);

private:
	uint64_t get_chunk_key(int x, int z) const;
};

}  // namespace lili

#endif
