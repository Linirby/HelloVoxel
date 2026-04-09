#ifndef RENDER_BUFFER_HPP
# define RENDER_BUFFER_HPP

# include <SDL3/SDL_gpu.h>
# include "geometry/mesher.hpp"

namespace lili {

class Buffer {
public:
	Buffer(SDL_GPUDevice *device, const Mesh &mesh);
	~Buffer();

	SDL_GPUBuffer *get_vertex_buffer() const;
	SDL_GPUBuffer *get_index_buffer() const;
	uint32_t get_index_count() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUBuffer *vertex_buffer = nullptr;
	SDL_GPUBuffer *index_buffer = nullptr;
	uint32_t index_count = 0;
};

}  // namespace lili

#endif
