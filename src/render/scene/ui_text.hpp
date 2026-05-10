#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/core/texture.hpp"
#include "world/material.hpp"
#include "render/core/gpu_mesh.hpp"
#include "render/scene/model.hpp"
#include "geometry/vec3.hpp"
#include "render/renderer.hpp"

namespace lili {

struct GlyphUV {
	float u0, v0, u1, v1;
};

class BitmapFont {
public:
	BitmapFont(
		SDL_GPUDevice *device,
		const std::string &atlas_path,
		int grid_cols = 16,
		int grid_rows = 6
	);

	Texture *get_texture() const;
	GlyphUV glyph_uv(char c) const;

private:
	std::unique_ptr<Texture> texture;
	int cols = 16;
	int rows = 6;
};

class UIText {
public:
	UIText(SDL_GPUDevice *device, BitmapFont *font, const std::string &text);

	float scale = 3.0f;
	float glyph_w = 5.0f;
	float glyph_h = 8.0f;
	float advance = 6.0f;

	void set_text(const std::string &value);
	void draw(Renderer *renderer, const Vec3 &position);

private:
	SDL_GPUDevice *device = nullptr;
	BitmapFont *font = nullptr;
	std::string text;

	std::unique_ptr<Material> material;
	std::unique_ptr<GPUMesh> mesh;
	Model model;

	void rebuild_mesh();
};

}  // namespace lili
