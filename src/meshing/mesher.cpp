#include "meshing/mesher.hpp"
#include "meshing/voxel_data.hpp"
#include "world/block.hpp"

namespace lili {

float AtlasProperties::get_uv_width() const {
	return 1.0f / cols;
}

float AtlasProperties::get_uv_height() const {
	return 1.0f / rows;
}

uint16_t AtlasProperties::get_index_from_pos(Vec2 pos) const {
	return pos.x + pos.y * cols;
}

MeshData ChunkMesher::generate_mesh(
	const Chunk &chunk, AtlasProperties atlas_props
) {
	MeshData mesh;

	for (int x = 0; x < Chunk::SIZE; ++x) {
		for (int y = 0; y < Chunk::SIZE; ++y) {
			for (int z = 0; z < Chunk::SIZE; ++z) {
				compute_faces(mesh, chunk, atlas_props, x, y, z);
			}
		}
	}
	return mesh;
}

void ChunkMesher::compute_vertices(
	float u_offset, float v_offset,
	AtlasProperties atlas_props, MeshData &mesh,
	int px, int py, int pz,
	int face,
	float nx, float ny, float nz,
	float material_id
) {
	for (int v = 0; v < 4; ++v) {
		float final_u = (
			u_offset +
			(face_uvs[v][0] * atlas_props.get_uv_width())
		);
		float final_v = (
			v_offset +
			(face_uvs[v][1] * atlas_props.get_uv_height())
		);

		mesh.vertices.push_back((Vertex){
			px + face_vertices[face][v][0],
			py + face_vertices[face][v][1],
			pz + face_vertices[face][v][2],
			nx, ny, nz,
			final_u, final_v,
			material_id
		});
	}
}

void ChunkMesher::compute_faces(
	MeshData &mesh,
	const Chunk &chunk,
	AtlasProperties atlas_props,
	int x, int y, int z
) {
	uint8_t block_id = chunk.get_block(x, y, z);
	if (block_id == 0) return;

	const BlockDefinition &block_def = (
		BlockRegistry::get().get_block(block_id)
	);

	float px = static_cast<float>(x);
	float py = static_cast<float>(y);
	float pz = static_cast<float>(z);

	float material_id = static_cast<float>(block_def.material_id);

	for (int face = 0; face < 6; ++face) {
		int check_x = x + face_normals[face][0];
		int check_y = y + face_normals[face][1];
		int check_z = z + face_normals[face][2];
		if (
			check_x < 0 || check_x >= Chunk::SIZE ||
			check_y < 0 || check_y >= Chunk::SIZE ||
			check_z < 0 || check_z >= Chunk::SIZE ||
			chunk.get_block(check_x, check_y, check_z) == 0
		) {
			uint32_t start_idx = static_cast<uint32_t>(
				mesh.vertices.size()
			);
			uint16_t tex_idx;
			switch (face) {
				case 0: tex_idx = block_def.top_texture; break;
				case 1: tex_idx = block_def.bottom_texture; break;
				case 2: tex_idx = block_def.right_texture; break;
				case 3: tex_idx = block_def.left_texture; break;
				case 4: tex_idx = block_def.front_texture; break;
				case 5: tex_idx = block_def.back_texture; break;
				default: tex_idx = 0; break;
			}

			float u_offset = (
				(tex_idx % atlas_props.cols) *
				atlas_props.get_uv_width()
			);
			float v_offset = (
				static_cast<int>(tex_idx / atlas_props.cols) *
				atlas_props.get_uv_height()
			);

			float nx = face_normals[face][0];
			float ny = face_normals[face][1];
			float nz = face_normals[face][2];

			compute_vertices(
				u_offset, v_offset,
				atlas_props, mesh,
				px, py, pz,
				face,
				nx, ny, nz,
				material_id
			);

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

}  // namespace lili
