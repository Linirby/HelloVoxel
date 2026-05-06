#include "meshing/primitives.hpp"

namespace lili {

MeshData create_unit_quad() {
	MeshData quad;

	quad.vertices = {
		(Vertex){
			.x = -0.5f, .y = -0.5f, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = 0.0f, .v = 0.0f,
			.material_id = 0.0f
		},
		(Vertex){
			.x = 0.5f, .y = -0.5f, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = 1.0f, .v = 0.0f,
			.material_id = 0.0f
		},
		(Vertex){
			.x = 0.5f, .y = 0.5f, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = 1.0f, .v = 1.0f,
			.material_id = 0.0f
		},
		(Vertex){
			.x = -0.5f, .y = 0.5f, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = 0.0f, .v = 1.0f,
			.material_id = 0.0f
		}
	};
	quad.indices = {
		0, 1, 2,
		2, 3, 0
	};
	return quad;
}

}  // namespace lili
