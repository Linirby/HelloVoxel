#include "render/pipelines/ui_pipeline.hpp"

#include <vector>
#include <stdexcept>

#include "render/core/shader.hpp"

namespace lili {

UIPipeline::UIPipeline(
	SDL_GPUDevice *device, SDL_Window *window, Shader *shader
) {
	this->device = device;
	this->window = window;
	this->shader = shader;

	SDL_GPUVertexBufferDescription vertex_buffer_desc{
		.slot = 0,
		.pitch = sizeof(float) * 8,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};
	std::vector<SDL_GPUVertexAttribute> vertex_attributes{
		(SDL_GPUVertexAttribute){
			.location = 0,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset = 0
		},
		(SDL_GPUVertexAttribute){
			.location = 2,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
			.offset = sizeof(float) * 6
		}
	};
	SDL_GPUColorTargetDescription color_target_desc{
		.format = SDL_GetGPUSwapchainTextureFormat(device, window),
		.blend_state = {
			.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
			.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
			.color_blend_op = SDL_GPU_BLENDOP_ADD,
			.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
			.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
			.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
			.color_write_mask = SDL_GPU_COLORCOMPONENT_R |
				SDL_GPU_COLORCOMPONENT_G |
				SDL_GPU_COLORCOMPONENT_B |
				SDL_GPU_COLORCOMPONENT_A,
			.enable_blend = true,
			.enable_color_write_mask = true,
		}
	};
	SDL_GPUGraphicsPipelineCreateInfo create_info{
		.vertex_shader = shader->get_vertex(),
		.fragment_shader = shader->get_fragment(),
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vertex_buffer_desc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertex_attributes.data(),
			.num_vertex_attributes = static_cast<uint32_t>(
				vertex_attributes.size()
			)
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = {
			.fill_mode = SDL_GPU_FILLMODE_FILL,
			.cull_mode = SDL_GPU_CULLMODE_NONE,
			.front_face = SDL_GPU_FRONTFACE_CLOCKWISE,
		},
		.multisample_state = {
			.sample_count = SDL_GPU_SAMPLECOUNT_1,
		},
		.depth_stencil_state = {
			.compare_op = SDL_GPU_COMPAREOP_ALWAYS,
			.back_stencil_state = { SDL_GPU_STENCILOP_ZERO },
			.front_stencil_state = { SDL_GPU_STENCILOP_ZERO },
			.compare_mask = 0,
			.write_mask = 0,
			.enable_depth_test = false,
			.enable_depth_write = false,
			.enable_stencil_test = false
		},
		.target_info = {
			.color_target_descriptions = &color_target_desc,
			.num_color_targets = 1,
			.depth_stencil_format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
			.has_depth_stencil_target = true
		},
		.props = 0
	};
	pipeline = SDL_CreateGPUGraphicsPipeline(device, &create_info);
	if (!pipeline) {
		throw std::runtime_error(
			"UI graphics pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

UIPipeline::~UIPipeline() {
	if (pipeline) SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
}

SDL_GPUGraphicsPipeline *UIPipeline::get_sdl_pipeline() {
	return pipeline;
}

}  // namespace lili