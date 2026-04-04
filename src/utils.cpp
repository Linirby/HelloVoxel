#include <fstream>
#include <cstring>
#include <SDL3_image/SDL_image.h>

#include "utils.hpp"

namespace Utils {

CodeInfo get_code_info(std::string filename) {
	CodeInfo code_info;

	std::ifstream vert_file(
		filename, std::ios::ate | std::ios::binary
	);
	if (!vert_file.is_open()) {
		throw std::runtime_error("Failed to open file! \"" + filename + "\"");
	}
	code_info.size = vert_file.tellg();
	vert_file.seekg(0);
	code_info.buffer.resize(code_info.size);
	vert_file.read(code_info.buffer.data(), code_info.size);
	vert_file.close();
	return code_info;
}

void transfer_buffer_to_gpu(
	SDL_GPUDevice *device,
	void *data,
	SDL_GPUBuffer *buffer,
	uint32_t data_size
) {
	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = data_size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, data, data_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
	SDL_GPUCommandBuffer *upload_cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(upload_cmd);
	SDL_GPUTransferBufferLocation src{
		.transfer_buffer = transfer_buffer,
		.offset = 0
	};
	SDL_GPUBufferRegion dst{
		.buffer = buffer,
		.offset = 0,
		.size = data_size
	};
	SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

void load_img_to_gpu_texture(
	SDL_GPUDevice *device, SDL_GPUTexture **texture, std::string filename
) {
	SDL_Surface *temp_surface = IMG_Load(filename.c_str());
	if (!temp_surface) {
		throw std::runtime_error("Failed to load image " + filename);
	}
	SDL_Surface *surface = SDL_ConvertSurface(
		temp_surface, SDL_PIXELFORMAT_ABGR8888
	);
	SDL_DestroySurface(temp_surface);
	SDL_GPUTextureCreateInfo texture_create_info{
		.type = SDL_GPU_TEXTURETYPE_2D,
		.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
		.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
		.width = static_cast<uint32_t>(surface->w),
		.height = static_cast<uint32_t>(surface->h),
		.layer_count_or_depth = 1,
		.num_levels = 1,
		.sample_count = SDL_GPU_SAMPLECOUNT_1,
		.props = 0
	};
	*texture = SDL_CreateGPUTexture(device, &texture_create_info);
	uint32_t image_size = surface->w * surface->h * 4;
	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = image_size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, surface->pixels, image_size);
	SDL_UnmapGPUTransferBuffer(device, transfer_buffer);
	SDL_GPUCommandBuffer *cmd = SDL_AcquireGPUCommandBuffer(device);
	SDL_GPUCopyPass *copy_pass = SDL_BeginGPUCopyPass(cmd);
	SDL_GPUTextureTransferInfo src{
		.transfer_buffer = transfer_buffer,
		.offset = 0
	};
	SDL_GPUTextureRegion dst{
		.texture = *texture,
		.w = static_cast<uint32_t>(surface->w),
		.h = static_cast<uint32_t>(surface->h),
		.d = 1
	};
	SDL_UploadToGPUTexture(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
	SDL_DestroySurface(surface);
}

} // namespace Utils
