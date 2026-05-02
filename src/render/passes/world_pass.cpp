#include "render/passes/world_pass.hpp"

#include "render/scene/model.hpp"

namespace lili {

WorldPass::WorldPass(
	SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline, Shader *shader
) {
	this->device = device;
	this->pipeline = pipeline;
	this->shader = shader;
}

void WorldPass::render(
	SDL_GPURenderPass *current_render_pass,
	SDL_GPUCommandBuffer *current_cmd_buffer,
	const Mat4 &proj_view,
	const std::vector<DrawCommand> &queue
) {
	if (queue.empty()) return;
	
	SDL_BindGPUGraphicsPipeline(current_render_pass, pipeline);
	for (const DrawCommand &cmd : queue) {
		Mat4 mvp = proj_view * cmd.transform;
		SDL_PushGPUVertexUniformData(current_cmd_buffer, 0, &mvp, sizeof(Mat4));

		SDL_GPUBufferBinding vertex_binding{
			.buffer = cmd.model.mesh->get_vertex(),
			.offset = 0
		};
		SDL_BindGPUVertexBuffers(current_render_pass, 0, &vertex_binding, 1);

		SDL_GPUBufferBinding index_binding{
			.buffer = cmd.model.mesh->get_index(),
			.offset = 0
		};
		SDL_BindGPUIndexBuffer(
			current_render_pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_32BIT
		);

		SDL_GPUTextureSamplerBinding atlas_texture_sampler_binding{
			.texture = cmd.model.texture->get_texture(),
			.sampler = cmd.model.texture->get_sampler()
		};
		SDL_BindGPUFragmentSamplers(
			current_render_pass, 0, &atlas_texture_sampler_binding, 1
		);

		SDL_DrawGPUIndexedPrimitives(
			current_render_pass,
			cmd.model.mesh->get_index_count(),
			1, 0, 0, 0
		);
	}
}

}  // namespace lili
