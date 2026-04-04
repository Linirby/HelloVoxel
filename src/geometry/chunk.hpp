#ifndef GEOMETRY_CHUNK_HPP
# define GEOMETRY_CHUNK_HPP

# include <cstdint>

class Chunk {
public:
	static constexpr int SIZE = 16;
	uint8_t blocks[SIZE * SIZE * SIZE];

	uint8_t get_block(int x, int y, int z) const;
};

#endif
