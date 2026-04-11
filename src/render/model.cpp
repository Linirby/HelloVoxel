#include "render/model.hpp"

namespace lili {

Model::Model(GPUMesh *mesh, Texture *texture) {
	this->mesh = mesh;
	this->texture = texture;
}

Model::~Model() {
	delete texture;
	delete mesh;
}

}  // namespace lili
