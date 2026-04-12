#include "geometry/primitives.hpp"

namespace lili {

MeshData create_unit_quad() {
	MeshData quad;

	quad.vertices = {
		(Vertex){ -0.5f, -0.5f, 0.0f,  0.0f, 0.0f },  // top-left
		(Vertex){  0.5f, -0.5f, 0.0f,  1.0f, 0.0f },  // top-right
		(Vertex){  0.5f,  0.5f, 0.0f,  1.0f, 1.0f },  // bottom-right
		(Vertex){ -0.5f,  0.5f, 0.0f,  0.0f, 1.0f }   // bottom-left
	};
	quad.indices = {
		0, 1, 2,
		2, 3, 0
	};
	return quad;
}

}  // namespace lili
