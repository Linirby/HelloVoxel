#include <SDL3/SDL.h>
#include <cmath>
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

	Vec3 flat_front = Vec3{ cam_front.x, 0.0f, cam_front.z }.normalized();
	Vec3 flat_right = Vec3{ cam_right.x, 0.0f, cam_right.z }.normalized();

	Vec3 move_dir = Vec3{ 0.0f, 0.0f, 0.0f };
	if (keys[SDL_SCANCODE_W]) move_dir += flat_front;
	if (keys[SDL_SCANCODE_S]) move_dir -= flat_front;
	if (keys[SDL_SCANCODE_D]) move_dir += flat_right;
	if (keys[SDL_SCANCODE_A]) move_dir -= flat_right;

	float current_speed = (
		keys[SDL_SCANCODE_LSHIFT] && keys[SDL_SCANCODE_W] ?
		run_speed : walk_speed
	);

	float target_vel_x = 0.0f;
	float target_vel_z = 0.0f;
	move_dir = move_dir.normalized();
	target_vel_x = move_dir.x * current_speed;
	target_vel_z = move_dir.z * current_speed;

	float control_speed = is_grounded ? 15.0f : 2.0f;

	velocity.x += (target_vel_x - velocity.x) * control_speed * dt;
	velocity.z += (target_vel_z - velocity.z) * control_speed * dt;

	if (keys[SDL_SCANCODE_SPACE] && is_grounded) {
		velocity.y = jump_power;
		is_grounded = false;
	}
}

void Player::update_physics(float dt, const Chunk &chunk) {
	if (mode == PlayerMode::Spectator) return;

	velocity.y += gravity * dt;
	
	Vec3 next_x = position;
	next_x.x += velocity.x * dt;
	if (!check_collision(next_x, chunk)) {
		position.x = next_x.x;
	} else {
		velocity.x = 0.0f;
	}

	Vec3 next_y = position;
	next_y.y += velocity.y * dt;
	is_grounded = false;
	if (!check_collision(next_y, chunk)) {
		position.y = next_y.y;
	} else {
		if (velocity.y < 0.0f) {
			is_grounded = true;
			position.y = std::floor(position.y);
		}
		velocity.y = 0.0f;
	}

	Vec3 next_z = position;
	next_z.z += velocity.z * dt;
	if (!check_collision(next_z, chunk)) {
		position.z = next_z.z;
	} else {
		velocity.z = 0.0f;
	}
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

bool Player::check_collision(const Vec3 &test_pos, const Chunk &chunk) const {
	float pad = 0.05f;
	float min_x = test_pos.x - (width / 2.0f) + pad;
	float max_x = test_pos.x + (width / 2.0f) - pad;

	float min_y = test_pos.y;
	float max_y = test_pos.y + height;

	float min_z = test_pos.z - (width / 2.0f) + pad;
	float max_z = test_pos.z + (width / 2.0f) - pad;

	for (int x = std::floor(min_x); x <= std::floor(max_x); ++x) {
		for (int y = std::floor(min_y); y <= std::floor(max_y); ++y) {
			for (int z = std::floor(min_z); z <= std::floor(max_z); ++z) {
				if (
					x < 0 || x >= Chunk::SIZE ||
					y < 0 || y >= Chunk::SIZE ||
					z < 0 || z >= Chunk::SIZE
				) {
					continue;
				}

				if (chunk.get_block(x, y, z) != 0) {
					return true;
				}
			}
		}
	}

	return false;
}

}  // namespace lili
