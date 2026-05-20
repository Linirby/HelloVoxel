/// GPU texture wrapper.
#pragma once

#include <SDL3/SDL_gpu.h>
#include <string>

namespace lili {

class Texture {
public:
	/// Create a texture from an image path.
	Texture(SDL_GPUDevice *device, const std::string &img_path);
	/// Destroy texture resources.
	~Texture();

	/// Get the underlying GPU texture.
	SDL_GPUTexture *get_texture() const;
	/// Get the sampler used for this texture.
	SDL_GPUSampler *get_sampler() const;

private:
	SDL_GPUDevice *device = nullptr;
	SDL_GPUTexture *texture = nullptr;
	SDL_GPUSampler *sampler = nullptr;

	void transfer_to_gpu(SDL_Surface *surface);
};

}  // namespace lili
