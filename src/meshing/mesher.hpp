#pragma once

#include "meshing/primitives.hpp"
#include "world/chunk.hpp"

namespace lili {

struct ChunkMesher {
	static MeshData generate_mesh(const Chunk &chunk);
};

}  // namespace lili
