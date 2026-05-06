#include "render/passes/world_pass.hpp"

#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

#include "render/scene/model.hpp"

namespace lili {

namespace {

struct MaterialGPU {
	float color_tint[4];
	float roughness;
	float metallic;
	float emission;
	float padding;
};

SDL_GPUBuffer *create_materials_buffer(SDL_GPUDevice *device) {
	const MaterialRegistry &registry = MaterialRegistry::get();
	const size_t material_count = registry.material_count();
	if (material_count == 0) {
		throw std::runtime_error("Material registry is empty.");
	}

	std::vector<MaterialGPU> gpu_materials;
	gpu_materials.reserve(material_count);
	for (size_t i = 0; i < material_count; ++i) {
		const MaterialProps &props = registry.get_material(
			static_cast<uint16_t>(i)
		).properties;
		gpu_materials.push_back({
			{
				props.color_tint.x,
				props.color_tint.y,
				props.color_tint.z,
				props.color_tint.w
			},
			props.roughness,
			props.metallic,
			props.emission,
			props.padding
		});
	}

	const uint32_t size = static_cast<uint32_t>(
		gpu_materials.size() * sizeof(MaterialGPU)
	);
	SDL_GPUBufferCreateInfo materials_buffer_createinfo{
		.usage = SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ,
		.size = size,
		.props = 0
	};
	SDL_GPUBuffer *materials_buffer = SDL_CreateGPUBuffer(
		device, &materials_buffer_createinfo
	);
	if (!materials_buffer) {
		throw std::runtime_error(
			"Materials storage buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	if (!transfer_buffer) {
		SDL_ReleaseGPUBuffer(device, materials_buffer);
		throw std::runtime_error(
			"Materials transfer buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}

	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	if (!map) {
		SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
		SDL_ReleaseGPUBuffer(device, materials_buffer);
		throw std::runtime_error(
			"Failed to map materials transfer buffer!\n-> " +
			std::string(SDL_GetError())
		);
	}
	std::memcpy(map, gpu_materials.data(), size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);

	SDL_GPUCommandBuffer *upload_cmd = SDL_AcquireGPUCommandBuffer(device);
	if (!upload_cmd) {
		SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
		SDL_ReleaseGPUBuffer(device, materials_buffer);
		throw std::runtime_error(
			"Failed to acquire command buffer for materials upload!"
		);
	}
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(upload_cmd);
	SDL_GPUTransferBufferLocation src{
		.transfer_buffer = transfer_buffer,
		.offset = 0
	};
	SDL_GPUBufferRegion dst{
		.buffer = materials_buffer,
		.offset = 0,
		.size = size
	};
	SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);

	return materials_buffer;
}

}  // namespace

WorldPass::WorldPass(
	SDL_GPUDevice *device, SDL_GPUGraphicsPipeline *pipeline, Shader *shader
) {
	this->device = device;
	this->pipeline = pipeline;
	this->shader = shader;
	this->materials_buffer = create_materials_buffer(device);
}

WorldPass::~WorldPass() {
	if (materials_buffer) {
		SDL_ReleaseGPUBuffer(device, materials_buffer);
	}
}

void WorldPass::render(
	SDL_GPURenderPass *current_render_pass,
	SDL_GPUCommandBuffer *current_cmd_buffer,
	const Mat4 &proj_view,
	const std::vector<DrawCommand> &queue
) {
	if (queue.empty()) return;
	
	SDL_BindGPUGraphicsPipeline(current_render_pass, pipeline);
	SDL_BindGPUFragmentStorageBuffers(current_render_pass, 0, &materials_buffer, 1);

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
		// SDL_PushGPUFragmentUniformData(
		// 	current_cmd_buffer,
		// 	0,
		// 	&cmd.model.material->properties,
		// 	sizeof(MaterialProps)
		// );

		SDL_DrawGPUIndexedPrimitives(
			current_render_pass,
			cmd.model.mesh->get_index_count(),
			1, 0, 0, 0
		);
	}
}

}  // namespace lili
