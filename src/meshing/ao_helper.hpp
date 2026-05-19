#pragma once

#include <cstdint>
#include "meshing/chunk_neighborhood.hpp"

namespace lili {

bool is_solid(const ChunkNeighborhood &neighborhood, int x, int y, int z);
uint8_t vertex_ao(bool s1, bool s2, bool corner);
uint8_t sample_vertex_ao(
	const ChunkNeighborhood &neighborhood,
	int fx, int fy, int fz, int face, int corner
);
void sample_face_ao(
	const ChunkNeighborhood &neighborhood,
	int fx, int fy, int fz, int face, uint8_t ao[4]
);

}  // namespace lili
