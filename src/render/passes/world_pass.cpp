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

		SDL_GPUTextureSamplerBinding texture_sampler_binding{
			.texture = cmd.model.material->albedo_map->get_texture(),
			.sampler = cmd.model.material->albedo_map->get_sampler()
		};
		SDL_BindGPUFragmentSamplers(
			current_render_pass, 0, &texture_sampler_binding, 1
		);

		SDL_PushGPUVertexUniformData(current_cmd_buffer, 0, &mvp, sizeof(Mat4));
		SDL_PushGPUFragmentUniformData(
			current_cmd_buffer,
			0,
			&cmd.model.material->properties,
			sizeof(MaterialProps)
		);

		SDL_DrawGPUIndexedPrimitives(
			current_render_pass,
			cmd.model.mesh->get_index_count(),
			1, 0, 0, 0
		);
	}
}

}  // namespace lili
