#include <SDL3/SDL.h>
#include "entity/player.hpp"

namespace lili {

void Player::process_keyboard(
	const bool *keys,
	const Vec3 &cam_front,
	const Vec3 &cam_right,
	const Vec3 &cam_up,
	float dt
) {
	float velocity = fly_speed * dt;

	if (keys[SDL_SCANCODE_W]) position += cam_front * velocity;
	if (keys[SDL_SCANCODE_S]) position -= cam_front * velocity;
	if (keys[SDL_SCANCODE_D]) position += cam_right * velocity;
	if (keys[SDL_SCANCODE_A]) position -= cam_right * velocity;

	if (keys[SDL_SCANCODE_SPACE]) position += cam_up * velocity;
	if (keys[SDL_SCANCODE_LSHIFT]) position -= cam_up * velocity;
}

}  // namespace lili
