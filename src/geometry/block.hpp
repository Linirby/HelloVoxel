#ifndef GEOMETRY_BLOCK_HPP
# define GEOMETRY_BLOCK_HPP

# include <cstdint>
# include <vector>

namespace lili {

struct BlockDefinition {
	uint8_t top_texture;
	uint8_t bottom_texture;
	uint8_t front_texture;
	uint8_t right_texture;
	uint8_t back_texture;
	uint8_t left_texture;
};

class BlockRegistry {
public:
	static BlockRegistry &get();
	const BlockDefinition &get_block(uint8_t block_id) const;

private:
	std::vector<BlockDefinition> blocks;

	BlockRegistry();
};

}  // namespace lili

#endif
