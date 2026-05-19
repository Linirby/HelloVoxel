/// GPU mesh wrapper.
#pragma once

#include <SDL3/SDL_gpu.h>

#include "meshing/primitives.hpp"

namespace lili {

class GPUMesh {
public:
	/// Create GPU buffers from mesh data.
	GPUMesh(SDL_GPUDevice *device, MeshData &mesh);
	/// Destroy GPU buffers.
	~GPUMesh();

	/// Get the vertex buffer.
	SDL_GPUBuffer *get_vertex() const;
	/// Get the index buffer.
	SDL_GPUBuffer *get_index() const;
	/// Get the index count.
	uint32_t get_index_count() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUBuffer *vertex_buffer = nullptr;
	SDL_GPUBuffer *index_buffer = nullptr;
	uint32_t index_count = 0;

	void transfer_to_gpu(void *data, SDL_GPUBuffer *buffer, uint32_t size);
};

}  // namespace lili
