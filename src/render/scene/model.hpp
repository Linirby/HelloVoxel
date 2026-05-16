#pragma once

#include "render/core/gpu_mesh.hpp"
#include "world/material.hpp"

namespace lili {

struct Model {
	GPUMesh *mesh;
	Material *material;

	Model();
	Model(GPUMesh *mesh, Material *material);
};

}  // namespace lili
