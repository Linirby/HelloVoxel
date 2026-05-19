/// Model scene object.
#pragma once

#include "render/core/gpu_mesh.hpp"
#include "world/material.hpp"

namespace lili {

struct Model {
	GPUMesh *mesh;
	Material *material;

	/// Create an empty model.
	Model();
	/// Create a model from mesh and material.
	Model(GPUMesh *mesh, Material *material);
};

}  // namespace lili
