#include "geometry/mesher.hpp"
#include "geometry/voxel_data.hpp"

Mesh ChunkMesher::generate_mesh(const Chunk &chunk) {
	Mesh mesh;

	for (int x = 0; x < Chunk::SIZE; ++x) {
		for (int y = 0; y < Chunk::SIZE; ++y) {
			for (int z = 0; z < Chunk::SIZE; ++z) {
				if (chunk.get_block(x, y, z) == 0) continue; // Skip air

				for (int face = 0; face < 3; ++face) {
					int check_x = x + lili::face_checks[face][0];
					int check_y = y + lili::face_checks[face][1];
					int check_z = z + lili::face_checks[face][2];
				}
			}
		}
	}
	return mesh;
}
