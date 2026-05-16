#include "render/scene/sprite.hpp"

namespace lili {

Sprite::Sprite() {
	texture = nullptr;
	material = nullptr;
	mesh = nullptr;
	model = {};
	position = {};
	scale = {};
	rotation = {};
}

void Sprite::set_texture(Renderer *renderer, const std::string &path) {
	texture = std::make_unique<Texture>(renderer->get_device(), path);
	material = std::make_unique<Material>(texture.get());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };

	MeshData mesh_data = create_unit_quad();
	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	model = Model(mesh.get(), material.get());
}

void Sprite::set_position(Vec3 position) {
	this->position = position;
}

void Sprite::set_scale(Vec3 scale) {
	this->scale = scale;
}

void Sprite::set_rotation(Vec3 rotation) {
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
