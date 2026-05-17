#pragma once

#include <cstdint>
#include <unordered_map>
#include <string>
#include <vector>

namespace lili {

struct BlockDefinition {
	uint16_t top_texture = 0;
	uint16_t bottom_texture = 0;
	uint16_t front_texture = 0;
	uint16_t right_texture = 0;
	uint16_t back_texture = 0;
	uint16_t left_texture = 0;
	uint16_t material_id = 0;
};

class BlockRegistry {
public:
	BlockRegistry(const BlockRegistry&) = delete;
	BlockRegistry& operator=(const BlockRegistry&) = delete;
	static BlockRegistry &get();

	uint16_t register_block(const std::string &key, const BlockDefinition &block);

	bool has_block(const std::string &key) const;
	uint16_t get_block_id(const std::string &key) const;
	const BlockDefinition &get_block(const std::string &key) const;
	const BlockDefinition &get_block(uint16_t block_id) const;
	const BlockDefinition &get_block(uint8_t block_id) const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<BlockDefinition> id_to_block;

	BlockRegistry();
};

}  // namespace lili
