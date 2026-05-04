#pragma once

#include "render/core/gpu_mesh.hpp"
#include "world/material.hpp"

namespace lili {

struct Model {
	GPUMesh *mesh = nullptr;
	Material *material = nullptr;

	Model() = default;
	Model(GPUMesh *mesh, Material *material);
};

}  // namespace lili
