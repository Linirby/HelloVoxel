#pragma once

#include <cstdint>
#include "world/chunk.hpp"

namespace lili {

bool is_solid(const Chunk &chunk, int x, int y, int z);
uint8_t vertex_ao(bool s1, bool s2, bool corner);
uint8_t sample_vertex_ao(
	const Chunk &chunk, int fx, int fy, int fz, int face, int corner
);
void sample_face_ao(
	const Chunk &chunk, int fx, int fy, int fz, int face, uint8_t ao[4]
);

}  // namespace lili
