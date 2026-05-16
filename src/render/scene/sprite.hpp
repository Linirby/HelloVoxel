#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>
#include <memory>

#include "render/renderer.hpp"
#include "render/core/texture.hpp"
#include "world/material.hpp"

namespace lili {

class Sprite {
public:
	Sprite();
	~Sprite() = default;

	void set_texture(Renderer *renderer, const std::string &path);
	void set_position(Vec3 position);
	void set_scale(Vec3 scale);
	void set_rotation(Vec3 rotation);
	void draw(Renderer *renderer);

private:
	std::unique_ptr<Texture> texture = nullptr;
	std::unique_ptr<Material> material = nullptr;
	std::unique_ptr<GPUMesh> mesh = nullptr;
	Model model;

	Vec3 position;
	Vec3 scale;
	Vec3 rotation;
};

}  // namespace lili
