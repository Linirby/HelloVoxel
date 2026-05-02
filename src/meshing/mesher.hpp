#ifndef MESHING_MESHER_HPP
# define MESHING_MESHER_HPP

# include "meshing/primitives.hpp"
# include "world/chunk.hpp"

namespace lili {

class ChunkMesher {
public:
	static MeshData generate_mesh(const Chunk &chunk);
};

}  // namespace lili

#endif  // MESHING_MESHER_HPP
