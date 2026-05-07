#include "world/block.hpp"

#include <stdexcept>
#include "world/material.hpp"

namespace lili {

BlockRegistry &BlockRegistry::get() {
	static BlockRegistry instance;
	return instance;
}

uint16_t BlockRegistry::register_block(
	const std::string &key, const BlockDefinition &block
) {
	auto it = key_to_id.find(key);
	if (it != key_to_id.end()) {
		id_to_block[it->second] = block;
		return it->second;
	}

	if (id_to_block.size() >= UINT16_MAX)
		throw std::runtime_error("Block registry reached uint16_t capacity.");

	uint16_t new_id = static_cast<uint16_t>(id_to_block.size());
	key_to_id[key] = new_id;
	id_to_block.push_back(block);
	return new_id;
}

bool BlockRegistry::has_block(const std::string &key) const {
	return key_to_id.contains(key);
}

uint16_t BlockRegistry::get_block_id(const std::string &key) const {
	auto it = key_to_id.find(key);
	if (it == key_to_id.end())
		throw std::runtime_error("Block key not found: " + key);
	return it->second;
}

const BlockDefinition &BlockRegistry::get_block(const std::string &key) const {
	return get_block(get_block_id(key));
}

const BlockDefinition &BlockRegistry::get_block(uint16_t block_id) const {
	if (block_id >= id_to_block.size())
		throw std::runtime_error("Block ID out of range.");
	return id_to_block[block_id];
}

const BlockDefinition &BlockRegistry::get_block(uint8_t block_id) const {
	return get_block(static_cast<uint16_t>(block_id));
}

BlockRegistry::BlockRegistry() {
	register_block("core:air", {
		.top_texture = 0,
		.bottom_texture = 0,
		.front_texture = 0,
		.right_texture = 0,
		.back_texture = 0,
		.left_texture = 0,
		.material_id = 0
	});
	register_block("core:debug", {
		.top_texture = 0,
		.bottom_texture = 1,
		.front_texture = 4,
		.right_texture = 5,
		.back_texture = 6,
		.left_texture = 7,
		.material_id = 0
	});
	register_block("core:log", {
		.top_texture = 8,
		.bottom_texture = 8,
		.front_texture = 12,
		.right_texture = 12,
		.back_texture = 12,
		.left_texture = 12,
		.material_id = MaterialRegistry::get().get_material_id("core:log_mat")
	});
}

}  // namespace lili
