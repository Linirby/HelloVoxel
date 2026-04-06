#ifndef GEOMETRY_MESHER_HPP
# define GEOMETRY_MESHER_HPP

# include <vector>
# include <cstdint>

# include "geometry/core.hpp"
# include "geometry/chunk.hpp"

namespace lili {

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

class ChunkMesher {
public:
	static Mesh generate_mesh(const Chunk &chunk);
};

}  // namespace lili

#endif
