#pragma once

#include "render/core/gpu_mesh.hpp"
#include "render/core/texture.hpp"

namespace lili {

struct Model {
	GPUMesh *mesh = nullptr;
	Texture *texture = nullptr;

	Model() = default;
	Model(GPUMesh *mesh, Texture *texture);
};

}  // namespace lili