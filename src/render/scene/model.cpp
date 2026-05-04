#include "render/scene/model.hpp"

namespace lili {

Model::Model(GPUMesh *mesh, Material *material) {
	this->mesh = mesh;
	this->material = material;
}

}  // namespace lili
