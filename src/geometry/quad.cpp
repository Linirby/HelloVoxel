#include <stdexcept>
#include <SDL3/SDL.h>

#include "geometry/quad.hpp"
#include "geometry/primitives.hpp"
#include "render/texture.hpp"

namespace lili {

Quad::Quad(SDL_GPUDevice *device, const std::string &texture_path) {
	texture = new Texture(device, texture_path);
	if (!texture)
		throw std::runtime_error("Crosshair texture creation failed!");

	MeshData quad_data = create_unit_quad();
	mesh = new GPUMesh(device, quad_data);
	if (!mesh)
		throw std::runtime_error("GPU Mesh creation failed!");

	model = new Model(mesh, texture);
	if (!model)
		throw std::runtime_error("Model creation failed!");
}

Quad::~Quad() {
	if (texture) delete texture;
	if (model) delete model;
}

void Quad::draw(Renderer *renderer) {
	Mat4 mat_translation = Mat4::translate(position);
	Mat4 mat_scale = Mat4::scale(scale);
	Mat4 mat_rotation = (
		Mat4::rotation_x(rotation.x) *
		Mat4::rotation_y(rotation.y) *
		Mat4::rotation_z(rotation.z)
	);
	Mat4 transformation = mat_translation * mat_scale * mat_rotation;
	renderer->submit(*model, transformation, RenderLayer::UI2D);
}

}  // namespace lili
