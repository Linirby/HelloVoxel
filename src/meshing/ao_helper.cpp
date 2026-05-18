#include "meshing/ao_helper.hpp"
#include "meshing/ao_data.hpp"

namespace lili {

bool is_solid(const Chunk &chunk, int x, int y, int z) {
	if (
		x < 0 || x >= Chunk::SIZE ||
		y < 0 || y >= Chunk::SIZE ||
		z < 0 || z >= Chunk::SIZE
	)
		return false;
	return chunk.get_block(x, y, z);
}

uint8_t vertex_ao(bool s1, bool s2, bool corner) {
	if (s1 && s2) return 3;
	return (uint8_t)s1 + (uint8_t)s2 + (uint8_t)corner;
}

uint8_t sample_vertex_ao(
	const Chunk &chunk, int fx, int fy, int fz, int face, int corner
) {
	const int *s1 = ao_neighbors[face][corner][0];
	const int *s2 = ao_neighbors[face][corner][1];
	const int *c = ao_neighbors[face][corner][2];

	return vertex_ao(
		is_solid(chunk, fx + s1[0], fy + s1[1], fz + s1[2]),
		is_solid(chunk, fx + s2[0], fy + s2[1], fz + s2[2]),
		is_solid(chunk, fx + c[0], fy + c[1], fz + c[2])
	);
}

void sample_face_ao(
	const Chunk &chunk, int fx, int fy, int fz, int face, uint8_t ao[4]
) {
	for (int corner = 0; corner < 4; ++corner)
		ao[corner] = sample_vertex_ao(chunk, fx, fy, fz, face, corner);
}

}  // namespace lili
