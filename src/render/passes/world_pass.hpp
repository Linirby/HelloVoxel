#pragma once

#include "render/passes/pass_types.hpp"
#include "render/core/shader.hpp"

namespace lili {

class WorldPass {
public:
	WorldPass(
		SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline, Shader *shader
	);

	void render(
		SDL_GPURenderPass *current_render_pass,
		SDL_GPUCommandBuffer *current_cmd_buffer,
		const Mat4 &proj_view,
		const std::vector<DrawCommand> &queue
	);

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUGraphicsPipeline *pipeline = nullptr;
	Shader *shader = nullptr;
};

}  // namespace lili
