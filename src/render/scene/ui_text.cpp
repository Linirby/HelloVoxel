#include "render/scene/ui_text.hpp"

namespace lili {

BitmapFont::BitmapFont() {
	texture = nullptr;
	cols = 16;
	rows = 16;
}

void BitmapFont::set_atlas_map(Renderer *renderer, const std::string &path) {
	texture = std::make_unique<Texture>(renderer->get_device(), path);
}

void BitmapFont::set_size(uint8_t cols, uint8_t rows) {
	this->cols = cols;
	this->rows = rows;
}

Texture *BitmapFont::get_texture() const {
	return texture.get();
}

GlyphUV BitmapFont::glyph_uv(char c) const {
	const int ascii = static_cast<unsigned char>(c);
	const int first = static_cast<unsigned char>(' ');
	int idx = ascii - first;
	if (ascii < first || ascii > 126)
		idx = '?' - first;
	const int current_x = idx % cols;
	const int current_y = idx / cols;
	const float delta_u = 1.0f / static_cast<float>(cols);
	const float delta_v = 1.0f / static_cast<float>(rows);

	return (GlyphUV){
		current_x * delta_u,
		current_y * delta_v,
		(current_x + 1) * delta_u,
		(current_y + 1) * delta_v
	};
}

UIText::UIText(
	Renderer *renderer, BitmapFont *font, const std::string &text
) {
	this->renderer = renderer;
	this->font = font;
	this->text = text;
	material = std::make_unique<Material>(font->get_texture());
	material->properties.color_tint = { 1.0f, 1.0f, 1.0f, 1.0f };
	rebuild_mesh();
}

void UIText::set_text(const std::string &value) {
	if (value == text) return;
	text = value;
	rebuild_mesh();
}

void UIText::draw(const Vec3 &position) {
	if (!mesh) return;
	Mat4 translation = Mat4::translate(position);
	Mat4 scaling = Mat4::scale({ scale, scale, 1.0f });
	Mat4 transform = translation * scaling;
	renderer->submit(model, transform, RenderLayer::UI2D);
}

void UIText::rebuild_mesh() {
	MeshData mesh_data;
	mesh_data.vertices.reserve(text.size() * 4);
	mesh_data.indices.reserve(text.size() * 6);

	float offset_x = 0.0f;
	float offset_y = 0.0f;
	uint32_t base = 0;

	for (char c : text) {
		if (c == '\n') {
			offset_x = 0.0;
			offset_y += glyph_h;
			continue;
		}

		GlyphUV uv = font->glyph_uv(c);

		mesh_data.vertices.push_back((Vertex){
			.x = offset_x, .y = offset_y, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = uv.u0, .v = uv.v0,
			.material_id = 0
		});
		mesh_data.vertices.push_back((Vertex){
			.x = offset_x + glyph_w, .y = offset_y, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = uv.u1, .v = uv.v0,
			.material_id = 0
		});
		mesh_data.vertices.push_back((Vertex){
			.x = offset_x + glyph_w, .y = offset_y - glyph_h, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = uv.u1, .v = uv.v1,
			.material_id = 0
		});
		mesh_data.vertices.push_back((Vertex){
			.x = offset_x, .y = offset_y - glyph_h, .z = 0.0f,
			.nx = 0.0f, .ny = 0.0f, .nz = 0.0f,
			.u = uv.u0, .v = uv.v1,
			.material_id = 0
		});

		mesh_data.indices.push_back(base + 0);
		mesh_data.indices.push_back(base + 1);
		mesh_data.indices.push_back(base + 2);
		mesh_data.indices.push_back(base + 2);
		mesh_data.indices.push_back(base + 3);
		mesh_data.indices.push_back(base + 0);
		base += 4;

		offset_x += advance;
	}

	mesh = std::make_unique<GPUMesh>(renderer->get_device(), mesh_data);
	model = Model(mesh.get(), material.get());
}

}  // namespace lili
