#pragma once

#include "meshing/primitives.hpp"
#include "meshing/chunk_neighborhood.hpp"
#include "geometry/vec2.hpp"

namespace lili {

struct AtlasProperties {
	int cols;
	int rows;

	float get_u() const;
	float get_v() const;
	
	uint16_t get_index_from_pos(Vec2 pos);
};

class ChunkMesher {
public:
	ChunkMesher(const ChunkNeighborhood &neighborhood, AtlasProperties atlas_props);
	MeshData generate_mesh();

private:
	const ChunkNeighborhood neighborhood;
	AtlasProperties atlas_props;
	MeshData mesh;

	void process_block(int x, int y, int z);
	void emit_face(int x, int y, int z, int face);
	void emit_indices(uint32_t start, const uint8_t ao[4]);
};

}  // namespace lili
