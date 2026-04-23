#ifndef RENDER_MODEL_HPP
# define RENDER_MODEL_HPP

# include "render/gpu_mesh.hpp"
# include "render/texture.hpp"

namespace lili {

struct Model {
	GPUMesh *mesh = nullptr;
	Texture *texture = nullptr;

	Model() = default;
	Model(GPUMesh *mesh, Texture *texture);
	~Model();
};

}  // namespace lili

#endif  // RENDER_MODEL_HPP
