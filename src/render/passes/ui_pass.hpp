/// UI render pass interface.
#pragma once

#include "render/passes/pass_types.hpp"

namespace lili {

class UIPass {
public:
	/// Create a UI render pass.
	UIPass(SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline);
	/// Destroy UI pass resources.
	~UIPass();

	/// Render the UI draw queue.
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
