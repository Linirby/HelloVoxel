#include "render/scene/sprite.hpp"

namespace lili {

Sprite::Sprite(
	SDL_GPUDevice *device,
	const std::string &texture_path,
	Vec3 position,
	Vec3 scale,
	Vec3 rotation
) {
	texture = std::make_unique<Texture>(device, texture_path);
	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(device, mesh_data);
	model = Model(mesh.get(), material.get());

	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
}

void Sprite::draw(Renderer *renderer) {
	Mat4 mat_transform = (
		Mat4::translate(position) *
		Mat4::scale(scale) *
		Mat4::rotation_xyz(rotation)
	);
	renderer->submit(model, mat_transform, RenderLayer::UI2D);
}

}  // namespace lili
