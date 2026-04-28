#include <SDL3/SDL_init.h>
#include <stdexcept>

#include "core/sdl_system.hpp"

namespace lili {

SDLSystem::SDLSystem() {
	if (!SDL_Init(SDL_INIT_VIDEO)) throw std::runtime_error(SDL_GetError());
}

SDLSystem::~SDLSystem() {
	SDL_QuitSubSystem(SDL_INIT_VIDEO);
	SDL_Quit();
}

}  // namespace lili
