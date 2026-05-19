/// Voxel map container.
#pragma once

#include <unordered_map>

#include "physics/world_interface.hpp"
#include "world/chunk.hpp"

namespace lili {

struct Map : public IVoxelWorld {
	/// Destroy the map and its chunks.
	~Map();

	std::unordered_map<uint64_t, Chunk> chunks;

	/// Set a block by global coordinates.
	void set_block_global(uint8_t id, int x, int y, int z);
	/// Get a block by global coordinates.
	uint8_t get_block_global(int x, int y, int z) const;
	/// Compute a chunk key from chunk coordinates.
	uint64_t get_chunk_key(int x, int y, int z) const;

	/// Check if a voxel is solid at the given coordinates.
	bool is_solid_at(int x, int y, int z) const override;
};

}  // namespace lili
