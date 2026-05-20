#include "meshing/mesher.hpp"

#include <stdexcept>

#include "meshing/voxel_data.hpp"
#include "world/block.hpp"
#include "meshing/ao_helper.hpp"

namespace lili {

float AtlasProperties::get_u() const { return 1.0f / cols; }
float AtlasProperties::get_v() const { return 1.0f / rows; }

uint16_t AtlasProperties::get_index_from_pos(Vec2 pos) {
	if (pos.x > cols - 1)
		throw std::runtime_error("x is out of atlas map");
	if (pos.y > rows - 1)
		throw std::runtime_error("y is out of atlas map");
	return pos.x + pos.y * cols;
}

static uint16_t get_face_texture(const BlockDefinition &def, int face) {
	switch (face) {
		case 0: return def.top_texture;
		case 1: return def.bottom_texture;
		case 2: return def.right_texture;
		case 3: return def.left_texture;
		case 4: return def.front_texture;
		case 5: return def.back_texture;
		default: return 0;
	}
}

ChunkMesher::ChunkMesher(
	const ChunkNeighborhood &neighborhood, AtlasProperties atlas_props
) : neighborhood(neighborhood), atlas_props(atlas_props) {}

MeshData ChunkMesher::generate_mesh() {
	for (int x = 0; x < Chunk::SIZE; ++x)
		for (int y = 0; y < Chunk::SIZE; ++y)
			for (int z = 0; z < Chunk::SIZE; ++z)
				process_block(x, y, z);
	return mesh;
}

void ChunkMesher::process_block(int x, int y, int z) {
	if (neighborhood.get_block(x, y, z) == 0)
		return;
	for (int face = 0; face < 6; ++face)
		emit_face(x, y, z, face);
}

void ChunkMesher::emit_face(int x, int y, int z, int face) {
	int cx = x + face_normals[face][0];
	int cy = y + face_normals[face][1];
	int cz = z + face_normals[face][2];

	if (neighborhood.get_block(cx, cy, cz) != 0)
		return;

	const BlockDefinition &def = BlockRegistry::get().get_block(
		static_cast<uint8_t>(neighborhood.get_block(x, y, z))
	);

	uint16_t tex_idx = get_face_texture(def, face);
	float u_off = (tex_idx % atlas_props.cols) * atlas_props.get_u();
	float v_off = (int)(tex_idx / atlas_props.cols) * atlas_props.get_v();
	float mat_id = static_cast<float>(def.material_id);
	float nx = static_cast<float>(face_normals[face][0]);
	float ny = static_cast<float>(face_normals[face][1]);
	float nz = static_cast<float>(face_normals[face][2]);

	uint8_t ao[4];
	sample_face_ao(neighborhood, cx, cy, cz, face, ao);

	uint32_t start = static_cast<uint32_t>(mesh.vertices.size());

	for (int v = 0; v < 4; ++v) {
		mesh.vertices.push_back({
			x + face_vertices[face][v][0],
            y + face_vertices[face][v][1],
            z + face_vertices[face][v][2],
            nx, ny, nz,
            u_off + face_uvs[v][0] * atlas_props.get_u(),
            v_off + face_uvs[v][1] * atlas_props.get_v(),
            mat_id,
            static_cast<float>(ao[v])
		});
	}
	emit_indices(start, ao);
}

void ChunkMesher::emit_indices(uint32_t start, const uint8_t ao[4]) {
	bool flip = (ao[0] + ao[2]) > (ao[1] + ao[3]);

	if (flip) {
		mesh.indices.insert(mesh.indices.end(), {
			static_cast<uint16_t>(start),
			static_cast<uint16_t>(start + 1),
			static_cast<uint16_t>(start + 3),
			static_cast<uint16_t>(start + 1),
			static_cast<uint16_t>(start + 2),
			static_cast<uint16_t>(start + 3),
		});
	} else {
		mesh.indices.insert(mesh.indices.end(), {
			static_cast<uint16_t>(start),
			static_cast<uint16_t>(start + 1),
			static_cast<uint16_t>(start + 2),
			static_cast<uint16_t>(start + 2),
			static_cast<uint16_t>(start + 3),
			static_cast<uint16_t>(start),
		});
	}
}

}  // namespace lili
