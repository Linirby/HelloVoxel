#ifndef RENDER_SCENE_MODEL_HPP
# define RENDER_SCENE_MODEL_HPP

# include "render/core/gpu_mesh.hpp"
# include "render/core/texture.hpp"

namespace lili {

struct Model {
	GPUMesh *mesh = nullptr;
	Texture *texture = nullptr;

	Model() = default;
	Model(GPUMesh *mesh, Texture *texture);
};

}  // namespace lili

#endif  // RENDER_SCENE_MODEL_HPP
