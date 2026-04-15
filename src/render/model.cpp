#include "render/model.hpp"

namespace lili {

Model::Model(GPUMesh *mesh, Texture *texture) {
	this->mesh = mesh;
	this->texture = texture;
}

Model::~Model() {
	if (texture) delete texture;
	if (mesh) delete mesh;
}

}  // namespace lili
