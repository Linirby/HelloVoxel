#include <string>
#include <stdexcept>

#include "render/passes/renderer.hpp"
#include "render/scene/model.hpp"

#include "math/mat4x4.hpp"
#include "math/utils.hpp"

namespace lili {

Renderer::Renderer(Window *window) {
	this->window = window->get_sdl_window();

	init_device();
	init_depth_texture();
	world_shader = new Shader(
		device, "shader/world.vert.spv", "shader/world.frag.spv"
	);
	if (!world_shader)
		throw std::runtime_error("World shader creation failed!");
	ui_shader = new Shader(
		device, "shader/ui.vert.spv", "shader/ui.frag.spv"
	);
	if (!world_shader)
		throw std::runtime_error("UI shader creation failed!");
	init_world_pipeline();
	init_ui_pipeline();
}

Renderer::~Renderer() {
	SDL_WaitForGPUIdle(device);

	if (ui_pipeline) SDL_ReleaseGPUGraphicsPipeline(device, ui_pipeline);
	if (world_pipeline) SDL_ReleaseGPUGraphicsPipeline(device, world_pipeline);
	if (ui_shader) delete ui_shader;
	if (world_shader) delete world_shader;
	if (depth_texture) SDL_ReleaseGPUTexture(device, depth_texture);
	if (device) SDL_DestroyGPUDevice(device);
}

bool Renderer::begin_frame(Camera camera) {
	current_cmd_buffer = SDL_AcquireGPUCommandBuffer(device);
	if (!current_cmd_buffer) {
		throw std::runtime_error("Failed to acquire command buffer!");
	}
	SDL_GPUTexture *swapchain_texture = nullptr;
	uint32_t width = 0;
	uint32_t height = 0;
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
		current_cmd_buffer = nullptr;
		current_render_pass = nullptr;
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

	int win_w, win_h;
	SDL_GetWindowSize(window, &win_w, &win_h);
	float aspect = static_cast<float>(win_w) / static_cast<float>(win_h);

	Mat4 view = camera.get_view_matrix();
	Mat4 proj = Mat4::perspective(
		deg_to_rad(camera.fov_y),
		(float)win_w / (float)win_h,
		camera.near_dist,
		camera.far_dist
	);
	projection_view_3d = proj * view;
	projection_2d = Mat4::orthographic(
		0.0f, static_cast<float>(win_w),
		static_cast<float>(win_h), 0.0f,
		-1.0f, 1.0f
	);
	return true;
}

void Renderer::submit(const Model &model, Mat4 transform, RenderLayer layer) {
	if (layer == RenderLayer::World3D) {
		world_queue.push_back({ model, transform });
	} else if (layer == RenderLayer::UI2D) {
		ui_queue.push_back({ model, transform });
	}
}

void Renderer::end_frame() {
	if (!world_queue.empty()) {
		SDL_BindGPUGraphicsPipeline(current_render_pass, world_pipeline);

		for (const DrawCommand &cmd : world_queue) {
			Mat4 mvp = projection_view_3d * cmd.transform;
			SDL_PushGPUVertexUniformData(
				current_cmd_buffer, 0, &mvp, sizeof(Mat4)
			);

			SDL_GPUBufferBinding vertex_binding{
				.buffer = cmd.model.mesh->get_vertex(),
				.offset = 0
			};
			SDL_BindGPUVertexBuffers(
				current_render_pass, 0, &vertex_binding, 1
			);
			SDL_GPUBufferBinding index_binding{
				.buffer = cmd.model.mesh->get_index(),
				.offset = 0
			};
			SDL_BindGPUIndexBuffer(
				current_render_pass,
				&index_binding,
				SDL_GPU_INDEXELEMENTSIZE_16BIT
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
	if (!ui_queue.empty()) {
		SDL_BindGPUGraphicsPipeline(current_render_pass, ui_pipeline);

		for (const DrawCommand &cmd : ui_queue) {
			Mat4 mvp = projection_2d * cmd.transform;
			SDL_PushGPUVertexUniformData(
				current_cmd_buffer, 0, &mvp, sizeof(Mat4)
			);

			SDL_GPUBufferBinding vertex_binding{
				.buffer = cmd.model.mesh->get_vertex(),
				.offset = 0
			};
			SDL_BindGPUVertexBuffers(
				current_render_pass, 0, &vertex_binding, 1
			);
			SDL_GPUBufferBinding index_binding{
				.buffer = cmd.model.mesh->get_index(),
				.offset = 0
			};
			SDL_BindGPUIndexBuffer(
				current_render_pass,
				&index_binding,
				SDL_GPU_INDEXELEMENTSIZE_16BIT
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

	world_queue.clear();
	ui_queue.clear();
	SDL_EndGPURenderPass(current_render_pass);
	SDL_SubmitGPUCommandBuffer(current_cmd_buffer);
}

SDL_GPUDevice *Renderer::get_device() const {
	return device;
}

void Renderer::init_device() {
	device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, true, nullptr);
	if (!device) {
		throw std::runtime_error(
			"Device creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
	if (!SDL_ClaimWindowForGPUDevice(device, this->window)) {
		throw std::runtime_error(
			"SDL_ClaimWindowForGPUDevice() failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	if (!SDL_SetGPUSwapchainParameters(
        device, 
        this->window, 
        SDL_GPU_SWAPCHAINCOMPOSITION_SDR, 
        SDL_GPU_PRESENTMODE_MAILBOX
    )) {
		SDL_SetGPUSwapchainParameters(
			device, 
			this->window, 
			SDL_GPU_SWAPCHAINCOMPOSITION_SDR, 
			SDL_GPU_PRESENTMODE_IMMEDIATE
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
			"Depth texture creation failed!\n-> " + std::string(SDL_GetError())
		);
	}
}

void Renderer::init_world_pipeline() {
	const int N_VERTEX_ATTRBS = 3;

	SDL_GPUVertexBufferDescription vertex_buffer_desc{
		.slot = 0,
		.pitch = sizeof(float) * 8,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};
	SDL_GPUVertexAttribute vertex_attributes[N_VERTEX_ATTRBS] = {
		(SDL_GPUVertexAttribute){
			.location = 0,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset = 0
		},
		(SDL_GPUVertexAttribute){
			.location = 1,
			.buffer_slot = 0,
			.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3,
			.offset = sizeof(float) * 3
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
		.vertex_shader = world_shader->get_vertex(),
		.fragment_shader = world_shader->get_fragment(),
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vertex_buffer_desc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertex_attributes,
			.num_vertex_attributes = N_VERTEX_ATTRBS
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
	world_pipeline = SDL_CreateGPUGraphicsPipeline(device, &create_info);
	if (!world_pipeline) {
		throw std::runtime_error(
			"Chunk graphics pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

void Renderer::init_ui_pipeline() {
	const int N_VERTEX_ATTRBS = 2;

	SDL_GPUVertexBufferDescription vertex_buffer_desc{
		.slot = 0,
		.pitch = sizeof(float) * 8,
		.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
		.instance_step_rate = 0
	};
	SDL_GPUVertexAttribute vertex_attributes[N_VERTEX_ATTRBS] = {
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
		.vertex_shader = ui_shader->get_vertex(),
		.fragment_shader = ui_shader->get_fragment(),
		.vertex_input_state = {
			.vertex_buffer_descriptions = &vertex_buffer_desc,
			.num_vertex_buffers = 1,
			.vertex_attributes = vertex_attributes,
			.num_vertex_attributes = N_VERTEX_ATTRBS
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
	ui_pipeline = SDL_CreateGPUGraphicsPipeline(device, &create_info);
	if (!ui_pipeline) {
		throw std::runtime_error(
			"HUD graphics pipeline creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
}

}  // namespace lili
