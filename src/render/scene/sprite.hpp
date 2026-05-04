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
	Sprite(
		SDL_GPUDevice *device,
		const std::string &texture_path,
		Vec3 position,
		Vec3 scale = {},
		Vec3 rotation = {}
	);

	Vec3 position;
	Vec3 scale;
	Vec3 rotation;

	void draw(Renderer *renderer);

private:
	std::unique_ptr<Texture> texture = nullptr;
	std::unique_ptr<Material> material = nullptr;
	std::unique_ptr<GPUMesh> mesh = nullptr;
	Model model;
};

}  // namespace lili
