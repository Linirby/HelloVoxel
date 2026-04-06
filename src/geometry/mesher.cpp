#include "geometry/mesher.hpp"
#include "geometry/voxel_data.hpp"

namespace lili {

Mesh ChunkMesher::generate_mesh(const Chunk &chunk) {
	Mesh mesh;

	for (int x = 0; x < Chunk::SIZE; ++x) {
		for (int y = 0; y < Chunk::SIZE; ++y) {
			for (int z = 0; z < Chunk::SIZE; ++z) {
				if (chunk.get_block(x, y, z) == 0) continue; // Skip air

				float px = static_cast<float>(x);
				float py = static_cast<float>(y);
				float pz = static_cast<float>(z);

				for (int face = 0; face < 6; ++face) {
					int check_x = x + face_checks[face][0];
					int check_y = y + face_checks[face][1];
					int check_z = z + face_checks[face][2];
					if (
						check_x < 0 || check_x >= Chunk::SIZE ||
						check_y < 0 || check_y >= Chunk::SIZE ||
						check_z < 0 || check_z >= Chunk::SIZE ||
						chunk.get_block(check_x, check_y, check_z) == 0
					) {
						uint16_t start_idx = static_cast<uint16_t>(
							mesh.vertices.size()
						);

						for (int v = 0; v < 4; ++v) {
							mesh.vertices.push_back({
								px + face_vertices[face][v][0],
								py + face_vertices[face][v][1],
								pz + face_vertices[face][v][2],
								0.0f, 0.0f
							});
						}

						mesh.indices.insert(mesh.indices.end(), {
							start_idx,
							static_cast<uint16_t>(start_idx + 1),
							static_cast<uint16_t>(start_idx + 2),
							static_cast<uint16_t>(start_idx + 2),
							static_cast<uint16_t>(start_idx + 3),
							start_idx
						});
					}
				}
			}
		}
	}
	return mesh;
}

}  // namespace lili
