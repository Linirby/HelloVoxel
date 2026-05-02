#pragma once

# include <SDL3/SDL_gpu.h>
# include <string>
# include <memory>

# include "render/passes/renderer.hpp"

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
	std::unique_ptr<Texture> texture;
	std::unique_ptr<GPUMesh> mesh;
	Model model;
};

}  // namespace lili