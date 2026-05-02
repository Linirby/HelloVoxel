#pragma once

#include <unordered_map>

#include "physics/world_interface.hpp"
#include "world/chunk.hpp"

namespace lili {

struct Map : public IVoxelWorld {
	~Map();

	std::unordered_map<uint64_t, Chunk> chunks;

	void set_block_global(uint8_t id, int x, int y, int z);
	uint8_t get_block_global(int x, int y, int z) const;
	uint64_t get_chunk_key(int x, int y, int z) const;

	bool is_solid_at(int x, int y, int z) const override;
};

}  // namespace lili
