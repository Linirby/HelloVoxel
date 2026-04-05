#include "geometry/mesher.hpp"

Mesh ChunkMesher::generate_mesh(const Chunk &chunk) {
	Mesh mesh;

	for (int x = 0; x < Chunk::SIZE; ++x) {
		for (int y = 0; y < Chunk::SIZE; ++y) {
			for (int z = 0; z < Chunk::SIZE; ++z) {
				if (chunk.get_block(x, y, z) == 0) continue; // Skip air

				// Check top neighbor (y + 1)
				if (
					y == Chunk::SIZE - 1 ||
					chunk.get_block(x, y + 1, z) == 0
				) {
					push_front_face(&mesh, {(float)x, (float)y, (float)z});
				}
				// Check front neighbor (z - 1)
				if (z == 0 || chunk.get_block(x, y, z - 1) == 0) {
					// Add Front
				}
				// Check bottom neighbor (y - 1)
				if (y == 0 || chunk.get_block(x, y - 1, z) == 0) {
					// Add Bottom
				}
				// Check right neighbor (x + 1)
				if (
					x == Chunk::SIZE - 1 ||
					chunk.get_block(x + 1, y, z) == 0
				) {
					// Add Right
				}
				// Check back neighbor (z + 1)
				if (
					z == Chunk::SIZE - 1 ||
					chunk.get_block(x, y, z + 1) == 0
				) {
					// Add Back
				}
				// Check left neighbor (x - 1)
				if (x == 0 || chunk.get_block(x - 1, y, z) == 0) {
					// Add Left
				}
			}
		}
	}
	return mesh;
}
void ChunkMesher::push_front_face(Mesh *mesh, lili::Vec3 pos) {
	mesh->vertices.push_back({ pos.x, pos.y, pos.z, 0.0f, 0.0f });
	mesh->vertices.push_back({ pos.x + 1.0f, pos.y, pos.z, 1.0f, 0.0f });
	mesh->vertices.push_back({ pos.x + 1.0f, pos.y - 1.0f, pos.z, 1.0f, 1.0f });
	mesh->vertices.push_back({ pos.x, pos.y - 1.0f, pos.z, 0.0f, 1.0f });
}
