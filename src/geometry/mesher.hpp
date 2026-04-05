#ifndef GEOMETRY_MESHER_HPP
# define GEOMETRY_MESHER_HPP

# include <vector>
# include <cstdint>

# include "geometry/core.hpp"
# include "geometry/chunk.hpp"
# include "math/vec3.hpp"

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<uint16_t> indices;
};

class ChunkMesher {
public:
	static Mesh generate_mesh(const Chunk &chunk);

private:
	static void push_front_face(Mesh *mesh, lili::Vec3 pos);
};

#endif
