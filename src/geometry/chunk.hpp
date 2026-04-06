#ifndef GEOMETRY_CHUNK_HPP
# define GEOMETRY_CHUNK_HPP

# include <cstdint>

namespace lili {

class Chunk {
	public:
		static constexpr int SIZE = 16;
		uint8_t blocks[SIZE * SIZE * SIZE] = { 0 };

		void set_block(uint8_t id, int x, int y, int z);
		uint8_t get_block(int x, int y, int z) const;
};

}  // namespace lili

#endif
