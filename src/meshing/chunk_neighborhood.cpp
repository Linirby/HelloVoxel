#include "meshing/chunk_neighborhood.hpp"

namespace lili {

uint16_t ChunkNeighborhood::get_block(int x, int y, int z) const {
	int cx = 1;
	int cy = 1;
	int cz = 1;

	if (x < 0) {
		cx = 0;
		x += Chunk::SIZE;
	} else if (x >= Chunk::SIZE) {
		cx = 2;
		x -= Chunk::SIZE;
	}
	if (y < 0) {
		cy = 0;
		y += Chunk::SIZE;
	} else if (y >= Chunk::SIZE) {
		cy = 2;
		y -= Chunk::SIZE;
	}
	if (z < 0) {
		cz = 0;
		z += Chunk::SIZE;
	} else if (z >= Chunk::SIZE) {
		cz = 2;
		z -= Chunk::SIZE;
	}

	const Chunk *neighbor = chunks[cx][cy][cz];
	if (!neighbor)
		return 0;

	return neighbor->get_block(x, y, z);
}

}  // namespace lili
