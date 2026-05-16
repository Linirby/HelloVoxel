#pragma once

#include "render/passes/pass_types.hpp"

namespace lili {

class ShadowPass {
public:
	ShadowPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);

	void render(
		SDL_GPUCommandBuffer *current_cmd_buffer,
		SDL_GPUTexture *shadow_map,
		const Mat4 &light_proj_view,
		const std::vector<DrawCommand> &queue
	);

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;
};

}  // namespace lili
