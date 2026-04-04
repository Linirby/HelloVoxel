#ifndef GEOMETRY_MESHER_HPP
# define GEOMETRY_MESHER_HPP

# include <vector>
# include <cstdint>

# include "geometry/core.hpp"
# include "geometry/chunk.hpp"

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

class ChunkMesher {
public:
	static Mesh generate_mesh(const Chunk &chunk) {
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
						// Add Top
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
	}
};

#endif
