#ifndef RENDER_GPU_MESH_HPP
# define RENDER_GPU_MESH_HPP

# include <SDL3/SDL_gpu.h>
# include "geometry/mesher.hpp"

namespace lili {

class GPUMesh {
public:
	GPUMesh(SDL_GPUDevice *device, MeshData &mesh);
	~GPUMesh();

	SDL_GPUBuffer *get_vertex() const;
	SDL_GPUBuffer *get_index() const;
	uint32_t get_index_count() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUBuffer *vertex_buffer = nullptr;
	SDL_GPUBuffer *index_buffer = nullptr;
	uint32_t index_count = 0;

	void transfer_to_gpu(void *data, SDL_GPUBuffer *buffer, uint32_t size);
};

}  // namespace lili

#endif  // RENDER_GPU_MESH_HPP
