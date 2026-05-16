#pragma once

#include "render/passes/pass_types.hpp"
namespace lili {

class WorldPass {
public:
	WorldPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);
	~WorldPass();

	void render(
		SDL_GPURenderPass *pass,
		SDL_GPUCommandBuffer *cmd,
		const Mat4 &cam_proj_view,
		const std::vector<DrawCommand> &queue
	);

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;
	SDL_GPUBuffer *materials_buffer = nullptr;
};

}  // namespace lili
