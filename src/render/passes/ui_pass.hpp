#pragma once

#include "render/passes/pass_types.hpp"

namespace lili {

class UIPass {
public:
	UIPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);
	~UIPass();

	void render(
		SDL_GPURenderPass *pass,
		SDL_GPUCommandBuffer *cmd,
		const Mat4 &proj_view,
		const std::vector<DrawCommand> &queue
	);

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;
	SDL_GPUBuffer *materials_buffer = nullptr;
};

}  // namespace lili
