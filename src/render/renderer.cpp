#include <string>
#include <stdexcept>
#include "render/renderer.hpp"
#include "math/mat4x4.hpp"
#include "math/utils.hpp"

namespace lili {

Renderer::Renderer(SDL_Window *window) {
	this->window = window;

	init_device();
	init_depth_texture();
	shader = new Shader(
		device, "shader/triangle.vert.spv", "shader/triangle.frag.spv"
	);
	if (!shader) {
		throw std::runtime_error("Shader creation failed!");
	}
	voxel_texture = new Texture(device, "assets/cube_atlas.png");
	if (!voxel_texture) {
		throw std::runtime_error("Texture creation failed!");
	}
	init_graphics_pipeline();
}

Renderer::~Renderer() {
	if (graphics_pipeline) SDL_ReleaseGPUGraphicsPipeline(
		device, graphics_pipeline
	);
	if (voxel_texture) delete voxel_texture;
	if (shader) delete shader;
	if (depth_texture) SDL_ReleaseGPUTexture(device, depth_texture);
	if (device) SDL_DestroyGPUDevice(device);
}

bool Renderer::begin_frame(Camera camera) {
	current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
	if (!current_cmd_buffer) {
		throw std::runtime_error("Failed to acquire command buffer!");
	}
	SDL_GPUTexture *swapchain_texture = nullptr;
	uint32_t width, height;
	bool success = SDL_WaitAndAcquireGPUSwapchainTexture(
		current_cmd_buffer,
		window,
		&swapchain_texture,
		&width,
		&height
	);
	if (!success) {
		throw std::runtime_error("Failed to acquire swapchain texture!");
	}
	if (!swapchain_texture) {
		SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
		return false;
	}
	SDL_GPUColorTargetInfo color_target_info{
		.texture = swapchain_texture,
		.clear_color = SDL_FColor{ 0.1f, 0.1f, 0.1f, 1.0f },
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_STORE,
	};
	SDL_GPUDepthStencilTargetInfo depth_target_info{
		.texture = depth_texture,
		.clear_depth = 1.0f,
		.load_op = SDL_GPU_LOADOP_CLEAR,
		.store_op = SDL_GPU_STOREOP_DONT_CARE,
		.stencil_load_op = SDL_GPU_LOADOP_DONT_CARE,
		.stencil_store_op = SDL_GPU_STOREOP_DONT_CARE,
		.cycle = false,
		.clear_stencil = 0
	};
	current_render_pass = SDL_BeginGPURenderPass(
		current_cmd_buffer,
		&color_target_info,
		1,
		&depth_target_info
	);
	Mat4 view = camera.get_view_matrix();
	int win_w, win_h;
	SDL_GetWindowSize(window, &win_w, &win_h);
	Mat4 proj = Mat4::perspective(
		deg_to_rad(70.0f),            // FOV Y (in rad)
		(float)win_w / (float)win_h,  // aspect ratio
		0.1f,                         // near distance unit
		100.0f                        // far distance unit
	);
	projection_view = proj * view;
	return true;
}

void Renderer::draw(
	const GPUMesh *mesh, const Texture *texture, Mat4 transform
) {
	lili::Mat4 mvp = projection_view * transform;
	SDL_PushGPUVertexUniformData(current_cmd_buffer, 0, &mvp, sizeof(lili::Mat4));
	SDL_BindGPUGraphicsPipeline(current_render_pass, graphics_pipeline);
	SDL_GPUBufferBinding vertex_binding{
		.buffer = mesh->get_vertex(),
		.offset = 0
	};
	SDL_BindGPUVertexBuffers(current_render_pass, 0, &vertex_binding, 1);
	SDL_GPUBufferBinding index_binding{
		.buffer = mesh->get_index(),
		.offset = 0
	};
	SDL_BindGPUIndexBuffer(
		current_render_pass, &index_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT
	);
	SDL_GPUTextureSamplerBinding atlas_texture_sampler_binding{
		.texture = voxel_texture->get_texture(),
		.sampler = voxel_texture->get_sampler()
	};
	SDL_BindGPUFragmentSamplers(
		current_render_pass, 0, &atlas_texture_sampler_binding, 1
	);
	SDL_DrawGPUIndexedPrimitives(
		current_render_pass, mesh->get_index_count(), 1, 0, 0, 0
	);
}

void Renderer::end_frame() {
	SDL_EndGPURenderPass(current_render_pass);
	SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
}

SDL_GPUDevice *Renderer::get_device() const {
	return device;
}

void Renderer::init_device() {
	device = SDL_CreateGPUDevice(
		SDL_GPU_SHADERFORMAT_SPIRV, true, "vulkan"
	);
	if (!device) {
		throw std::runtime_error(
			"core.device creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device, this->window)) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

void Renderer::init_depth_texture() {
	int win_w, win_h;
	SDL_GetWindowSize(window, &win_w, &win_h);
	SDL_GPUTextureCreateInfo depth_info{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_D16_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_DEPTH_STENCIL_TARGET,
		.width = static_cast<uint32_t>(win_w),
		.height = static_cast<uint32_t>(win_h),
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	depth_texture = SDL_CreateGPUTexture(device, &depth_info);
	if (!depth_texture) {
		throw std::runtime_error(
			"res.depth_texture creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

void Renderer::init_graphics_pipeline() {
	SDL_GPUVertexBufferDescription vertex_buffer_desc{
		.slot = 0,
		.pitch = sizeof(float) * 5,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};
	SDL_GPUVertexAttribute vertex_attributes[2] = {
		(SDL_GPUVertexAttribute){
			.location = 0,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset = 0
		},
		(SDL_GPUVertexAttribute){
			.location = 1,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
			.offset = sizeof(float) * 3
		}
	};
	SDL_GPUColorTargetDescription color_target_desc{
		.format = SDL_GetGPUSwapchainTextureFormat(device, window),
		.blend_state = {
			.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
			.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_DST_COLOR,
			.color_blend_op = SDL_GPU_BLENDOP_ADD,
			.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_COLOR,
			.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_DST_COLOR,
			.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
			.color_write_mask = SDL_GPU_COLORCOMPONENT_R |
				SDL_GPU_COLORCOMPONENT_G |
				SDL_GPU_COLORCOMPONENT_B |
				SDL_GPU_COLORCOMPONENT_A,
			.enable_blend = false,
			.enable_color_write_mask = true,
		}
	};
	SDL_GPUGraphicsPipelineCreateInfo create_info{
		.vertex_shader = shader->get_vertex(),
		.fragment_shader = shader->get_fragment(),
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vertex_buffer_desc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertex_attributes,
			.num_vertex_attributes = 2
		},
		.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST,
		.rasterizer_state = {
			.fill_mode = SDL_GPU_FILLMODE_FILL,
			.cull_mode = SDL_GPU_CULLMODE_BACK,
			.front_face = SDL_GPU_FRONTFACE_CLOCKWISE,
		},
		.multisample_state = {
			.sample_count = SDL_GPU_SAMPLECOUNT_1,
		},
		.depth_stencil_state = {
			.compare_op = SDL_GPU_COMPAREOP_LESS_OR_EQUAL,
			.back_stencil_state = { SDL_GPU_STENCILOP_ZERO },
			.front_stencil_state = { SDL_GPU_STENCILOP_ZERO },
			.compare_mask = 0,
			.write_mask = 0,
			.enable_depth_test = true,
			.enable_depth_write = true,
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
	graphics_pipeline = SDL_CreateGPUGraphicsPipeline(device, &create_info);
	if (!graphics_pipeline) {
		throw std::runtime_error(
			"pipe.graphics_pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

}  // namespace lili
