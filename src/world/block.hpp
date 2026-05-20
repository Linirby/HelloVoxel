/// Block registry and block definitions.
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
	/// Non-copyable.
	BlockRegistry(const BlockRegistry&) = delete;
	/// Non-copyable.
	BlockRegistry& operator=(const BlockRegistry&) = delete;
	/// Get the global block registry instance.
	static BlockRegistry &get();

	/// Register a block definition and return its ID.
	uint16_t register_block(const std::string &key, const BlockDefinition &block);

	/// Check if a block key is registered.
	bool has_block(const std::string &key) const;
	/// Get the number of registered blocks.
	uint16_t get_size() const;
	/// Get a block ID by key.
	uint16_t get_block_id(const std::string &key) const;
	/// Get a block definition by key.
	const BlockDefinition &get_block(const std::string &key) const;
	/// Get a block definition by ID.
	const BlockDefinition &get_block(uint16_t block_id) const;
	/// Get a block definition by ID (uint8 overload).
	const BlockDefinition &get_block(uint8_t block_id) const;

private:
	std::unordered_map<std::string, uint16_t> key_to_id;
	std::vector<BlockDefinition> id_to_block;

	BlockRegistry();
};

}  // namespace lili
