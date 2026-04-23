#ifndef GEOMETRY_QUAD_HPP
# define GEOMETRY_QUAD_HPP

# include "render/renderer.hpp"
# include "render/gpu_mesh.hpp"
# include "render/model.hpp"
# include "math/vec3.hpp"

namespace lili {

class Quad {
public:
	Quad(SDL_GPUDevice *device, const std::string &texture_path);
	~Quad();

	Model *model;
	Vec3 position = {};
	Vec3 rotation = {};
	Vec3 scale = {};

	void draw(Renderer *renderer);

private:
	GPUMesh *mesh;
	Texture *texture;
};

}  // namespace lili

#endif  // GEOMETRY_QUAD_HPP
