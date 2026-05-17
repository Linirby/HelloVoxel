#pragma once

#include "meshing/primitives.hpp"
#include "world/chunk.hpp"

namespace lili {

struct AtlasProperties {
	int cols;
	int rows;

	float get_uv_width() const;
	float get_uv_height() const;
};

struct ChunkMesher {
	static MeshData generate_mesh(
		const Chunk &chunk, AtlasProperties atlas_props
	);
};

}  // namespace lili
