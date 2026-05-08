#include "render/passes/shadow_pass.hpp"

#include <stdexcept>
#include <vector>

namespace lili {

ShadowPass::ShadowPass(
	SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline
) {
	this->device = device;
	this->pipeline = pipeline;
}

void ShadowPass::render(
	SDL_GPUCommandBuffer *cmd,
	SDL_GPUTexture *shadow_map,
	const Mat4 &light_proj_view,
	const std::vector<DrawCommand> &queue
) {
	if (queue.empty()) return;

	SDL_GPUDepthStencilTargetInfo depth_target{
		.texture = shadow_map,
		.clear_depth = 1.0f,
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
		.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
		.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
		.cycle = false,
		.clear_stencil = 0
	};
	SDL_GPURenderPass *pass = SDL_BeginGPURenderPass(
		cmd, nullptr, 0, &depth_target
	);
	if (!pass)
		throw std::runtime_error(
			"Shadow pass creation failed: " + std::string(SDL_GetError())
		);
	SDL_BindGPUGraphicsPipeline(pass, pipeline);

	for (const DrawCommand &draw_cmd : queue) {
		Mat4 light_mvp = light_proj_view * draw_cmd.transform;
		SDL_PushGPUVertexUniformData(cmd, 0, &light_mvp, sizeof(Mat4));

		SDL_GPUBufferBinding vertex_binding{
			.buffer = draw_cmd.model.mesh->get_vertex(),
			.offset = 0
		};
		SDL_BindGPUVertexBuffers(pass, 0, &vertex_binding, 1);

		SDL_GPUBufferBinding index_binding{
			.buffer = draw_cmd.model.mesh->get_index(),
			.offset = 0
		};
		SDL_BindGPUIndexBuffer(
			pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT
		);

		SDL_DrawGPUIndexedPrimitives(
			pass, draw_cmd.model.mesh->get_index_count(), 1, 0, 0, 0
		);
	}

	SDL_EndGPURenderPass(pass);
}

}  // namespace lili
