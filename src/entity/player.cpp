#include <SDL3/SDL.h>
#include "entity/player.hpp"

namespace lili {

void Player::process_input(
	const bool *keys,
	const Vec3 &cam_front, const Vec3 &cam_right, const Vec3 &cam_up,
	float dt
) {
	if (mode == PlayerMode::Spectator) {
		float current_fly_speed = fly_speed * dt;

		if (keys[SDL_SCANCODE_W]) position += cam_front * current_fly_speed;
		if (keys[SDL_SCANCODE_S]) position -= cam_front * current_fly_speed;
		if (keys[SDL_SCANCODE_D]) position += cam_right * current_fly_speed;
		if (keys[SDL_SCANCODE_A]) position -= cam_right * current_fly_speed;

		if (keys[SDL_SCANCODE_SPACE]) position += cam_up * current_fly_speed;
		if (keys[SDL_SCANCODE_LSHIFT]) position -= cam_up * current_fly_speed;

		return;
	}

	velocity.x = 0.0f;
	velocity.z = 0.0f;

	Vec3 flat_front = Vec3{ cam_front.x, 0.0f, cam_front.z }.normalized();
	Vec3 flat_right = Vec3{ cam_right.x, 0.0f, cam_right.z }.normalized();

	Vec3 move_dir = Vec3{ 0.0f, 0.0f, 0.0f };
	if (keys[SDL_SCANCODE_W]) move_dir += flat_front;
	if (keys[SDL_SCANCODE_S]) move_dir -= flat_front;
	if (keys[SDL_SCANCODE_D]) move_dir += flat_right;
	if (keys[SDL_SCANCODE_A]) move_dir -= flat_front;

	move_dir = move_dir.normalized();
	velocity.x = move_dir.x * speed;
	velocity.z = move_dir.z * speed;

	if (keys[SDL_SCANCODE_SPACE] && is_grounded) {
		velocity.y = jump_power;
		is_grounded = false;
	}
}

void Player::update_physics(float dt, const Chunk &chunk) {
	if (mode == PlayerMode::Spectator) return;
}

void Player::toggle_mode() {
	if (mode == PlayerMode::Physical) {
		mode = PlayerMode::Spectator;
		position.y += 0.5f;
		velocity = Vec3{ 0.0f, 0.0f, 0.0f };
	} else {
		mode = PlayerMode::Physical;
	}
}

}  // namespace lili
