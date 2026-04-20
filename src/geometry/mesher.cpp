#include "geometry/mesher.hpp"
#include "geometry/voxel_data.hpp"
#include "geometry/block.hpp"

namespace lili {

inline constexpr int ATLAS_COLS = 4;
inline constexpr int ATLAS_ROWS = 4;
constexpr float UV_W = 1.0f / ATLAS_COLS;
constexpr float UV_H = 1.0f / ATLAS_ROWS;

MeshData ChunkMesher::generate_mesh(const Chunk &chunk) {
	MeshData mesh;

	for (int x = 0; x < Chunk::SIZE; ++x) {
		for (int y = 0; y < Chunk::SIZE; ++y) {
			for (int z = 0; z < Chunk::SIZE; ++z) {

				uint8_t block_id = chunk.get_block(x, y, z);
				if (block_id == 0) continue; // Skip air

				const BlockDefinition &block_def = (
					BlockRegistry::get().get_block(block_id)
				);

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
						uint8_t tex_idx;
						switch (face) {
							case 0: tex_idx = block_def.top_texture; break;
							case 1: tex_idx = block_def.bottom_texture; break;
							case 2: tex_idx = block_def.right_texture; break;
							case 3: tex_idx = block_def.left_texture; break;
							case 4: tex_idx = block_def.front_texture; break;
							case 5: tex_idx = block_def.back_texture; break;
							default: tex_idx = 0; break;
						}

						float u_offset = (tex_idx % ATLAS_COLS) * UV_W;
						float v_offset = (
							(static_cast<int>(tex_idx / ATLAS_COLS)) * UV_H
						);

						for (int v = 0; v < 4; ++v) {
							float final_u = u_offset + (face_uvs[v][0] * UV_W);
							float final_v = v_offset + (face_uvs[v][1] * UV_H);

							mesh.vertices.push_back({
								px + face_vertices[face][v][0],
								py + face_vertices[face][v][1],
								pz + face_vertices[face][v][2],
								final_u, final_v
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
