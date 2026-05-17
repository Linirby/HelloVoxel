#pragma once

#include "meshing/primitives.hpp"
#include "world/chunk.hpp"
#include "geometry/vec2.hpp"

namespace lili {

struct AtlasProperties {
	int cols;
	int rows;

	float get_uv_width() const;
	float get_uv_height() const;
	uint16_t get_index_from_pos(Vec2 pos) const;
};

struct ChunkMesher {
	static MeshData generate_mesh(
		const Chunk &chunk, AtlasProperties atlas_props
	);
};

}  // namespace lili
