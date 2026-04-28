#include "world/block.hpp"

namespace lili {

BlockRegistry &BlockRegistry::get() {
	static BlockRegistry instance;
	return instance;
}

const BlockDefinition &BlockRegistry::get_block(uint8_t block_id) const {
	return blocks[block_id];
}

BlockRegistry::BlockRegistry() {

	// { Top, Bottom, Right, Left, Front, Back }
	// Index value is from left to right and top to bottom

	blocks.push_back({ 0, 0,  0,  0,  0,  0 });  // 0: air
	blocks.push_back({ 0, 1,  4,  5,  6,  7 });  // 1: debug
	blocks.push_back({ 8, 8, 12, 12, 12, 12 });  // 2: log
}

}  // namespace lili
