#include "render/pipelines/shadow_pipeline.hpp"

#include <stdexcept>

#include "render/core/shader.hpp"

namespace lili {

ShadowPipeline::ShadowPipeline(
	SDL_GPUDevice *device, SDL_Window *window, Shader *shader
) {
	this->device = device;
	this->window = window;
	this->shader = shader;

	SDL_GPUVertexBufferDescription vertex_buffer_desc{
		.slot = 0,
		.pitch = sizeof(float) * 9,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};
	SDL_GPUVertexAttribute vertex_attribute{
		.location = 0,
		.buffer_slot = 0,
		.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
		.offset = 0
	};

	SDL_GPUGraphicsPipelineCreateInfo create_info{
		.vertex_shader = shader->get_vertex(),
		.fragment_shader = shader->get_fragment(),
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vertex_buffer_desc,
			.num_vertex_buffers = 1,
			.vertex_attributes = &vertex_attribute,
			.num_vertex_attributes = 1
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = {
			.fill_mode = SDL_GPU_FILLMODE_FILL,
			.cull_mode = SDL_GPU_CULLMODE_NONE,
			.front_face = SDL_GPU_FRONTFACE_CLOCKWISE,
			.depth_bias_constant_factor = 0.75f,
			.depth_bias_clamp = 0.0f,
			.depth_bias_slope_factor = 0.75f,
			.enable_depth_bias = true,
			.enable_depth_clip = true
		},
		.depth_stencil_state = {
			.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
			.enable_depth_test = true,
			.enable_depth_write = true,
		},
		.target_info = {
			.color_target_descriptions = nullptr,
			.num_color_targets = 0,
			.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D32_FLOAT,
			.has_depth_stencil_target = true
		},
		.props = 0
	};
	pipeline = SDL_CreateGPUGraphicsPipeline(device, &create_info);
	if (!pipeline) {
		throw std::runtime_error(
			"Shadow graphics pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

ShadowPipeline::~ShadowPipeline() {
	if (pipeline) SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}

SDL_GPUGraphicsPipeline *ShadowPipeline::get_sdl_pipeline() {
	return pipeline;
}

}  // namespace lili
