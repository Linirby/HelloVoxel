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

enum BlockID : uint8_t {
	BLOCK_ID_AIR = 0,
	BLOCK_ID_DEBUG = 1,
	BLOCK_ID_LOG = 2
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

#endif  // GEOMETRY_BLOCK_HPP
