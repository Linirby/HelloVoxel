#pragma once

#include "meshing/primitives.hpp"
#include "world/chunk.hpp"
#include "geometry/vec2.hpp"

namespace lili {

struct AtlasProperties {
	int cols;
	int rows;

	float get_uv_width() const;
	float get_uv_height() const;
	uint16_t get_index_from_pos(Vec2 pos) const;
};

class ChunkMesher {
public:
	static MeshData generate_mesh(
		const Chunk &chunk, AtlasProperties atlas_props
	);

private:
	static void compute_vertex_ao(
		const Chunk &chunk,
		int fx, int fy, int fz,
		int face, int corner
	);
	static void compute_vertices(
		float u_offset, float v_offset,
		AtlasProperties atlas_props, MeshData &mesh,
		int px, int py, int pz,
		int face,
		float nx, float ny, float nz,
		float material_id
	);
	static void compute_faces(
		MeshData &mesh,
		const Chunk &chunk,
		AtlasProperties atlas_props,
		int x, int y, int z
	);
};

}  // namespace lili
