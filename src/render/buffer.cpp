#include <cstring>
#include <stdexcept>
#include "render/buffer.hpp"

namespace lili {

Buffer::Buffer(SDL_GPUDevice *device, Mesh &mesh) {
	this->device = device;
	index_count = static_cast<uint32_t>(mesh.indices.size());

	uint32_t vertices_buffer_size = (
		mesh.vertices.size() * sizeof(lili::Vertex)
	);
	SDL_GPUBufferCreateInfo vertices_buffer_create_info{
		.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
		.size = vertices_buffer_size,
		.props = 0
	};
	vertex_buffer = SDL_CreateGPUBuffer(
		this->device, &vertices_buffer_create_info
	);
	if (!vertex_buffer) {
		throw std::runtime_error(
			"vertex_buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	transfer_to_gpu(mesh.vertices.data(), vertex_buffer, vertices_buffer_size);

	uint32_t indices_buffer_size = mesh.indices.size() * sizeof(uint16_t);
	SDL_GPUBufferCreateInfo indices_buffer_create_info{
		.usage = SDL_GPU_BUFFERUSAGE_INDEX,
		.size = indices_buffer_size,
		.props = 0
	};
	index_buffer = SDL_CreateGPUBuffer(
		device, &indices_buffer_create_info
	);
	if (!index_buffer) {
		throw std::runtime_error(
			"index_buffer creation failed!\n-> " +
			std::string(SDL_GetError())
		);
	}
	transfer_to_gpu(
		mesh.indices.data(),
		index_buffer,
		indices_buffer_size
	);
}

Buffer::~Buffer() {
	if (index_buffer)
		SDL_ReleaseGPUBuffer(device, index_buffer);
	if (vertex_buffer)
		SDL_ReleaseGPUBuffer(device, vertex_buffer);
}

SDL_GPUBuffer *Buffer::get_vertex() const {
	return vertex_buffer;
}

SDL_GPUBuffer *Buffer::get_index() const {
	return index_buffer;
}

uint32_t Buffer::get_index_count() const {
	return index_count;
}

void Buffer::transfer_to_gpu(void *data, SDL_GPUBuffer *buffer, uint32_t size) {
	SDL_GPUTransferBufferCreateInfo transfer_buffer_info{
		.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
		.size = size,
		.props = 0
	};
	SDL_GPUTransferBuffer *transfer_buffer = SDL_CreateGPUTransferBuffer(
		device, &transfer_buffer_info
	);
	void *map = SDL_MapGPUTransferBuffer(device, transfer_buffer, false);
	std::memcpy(map, data, size);
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
		.size = size
	};
	SDL_UploadToGPUBuffer(copy_pass, &src, &dst, false);
	SDL_EndGPUCopyPass(copy_pass);
	SDL_SubmitGPUCommandBuffer(upload_cmd);
	SDL_ReleaseGPUTransferBuffer(device, transfer_buffer);
}

}  // namespace lili
