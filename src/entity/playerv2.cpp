#include <SDL3/SDL.h>
#include <cmath>
#include "entity/playerv2.hpp"

namespace lili {

void PlayerV2::process_input(
	const bool *keys,
	const Vec3 &cam_front, const Vec3 &cam_right, const Vec3 &cam_up,
	float dt
) {
	if (mode == PlayerMode::Spectator) {
		float current_fly_speed = max_speed * dt;

		if (keys[SDL_SCANCODE_W]) position += cam_front * current_fly_speed;
		if (keys[SDL_SCANCODE_S]) position -= cam_front * current_fly_speed;
		if (keys[SDL_SCANCODE_D]) position += cam_right * current_fly_speed;
		if (keys[SDL_SCANCODE_A]) position -= cam_right * current_fly_speed;

		if (keys[SDL_SCANCODE_SPACE]) position += cam_up * current_fly_speed;
		if (keys[SDL_SCANCODE_LSHIFT]) position -= cam_up * current_fly_speed;

		return;
	}

	// Crouch
	if (keys[SDL_SCANCODE_LCTRL]) {
		if (!is_crouching) {
			is_crouching = true;
			position.y += (standing_height - crouching_height);
			height = crouching_height;
		}
	} else {
		if (is_crouching) {
			is_crouching = false;
			position.y -= (standing_height - crouching_height);
			height = standing_height;
		}
	}

	// Wish direction
	Vec3 flat_front = Vec3{ cam_front.x, 0.0f, cam_front.z }.normalized();
	Vec3 flat_right = Vec3{ cam_right.x, 0.0f, cam_right.z }.normalized();

	Vec3 wish_dir = Vec3{ 0.0f, 0.0f, 0.0f };
	if (keys[SDL_SCANCODE_W]) wish_dir += flat_front;
	if (keys[SDL_SCANCODE_S]) wish_dir -= flat_front;
	if (keys[SDL_SCANCODE_D]) wish_dir += flat_right;
	if (keys[SDL_SCANCODE_A]) wish_dir -= flat_right;

	if (wish_dir.length() > 0.0f) {
		wish_dir = wish_dir.normalized();
	}

	// Ground
	if (is_grounded) {
		apply_friction(dt);
		float target_speed = is_crouching ? (max_speed * 0.4f) : max_speed;
		accelerate(wish_dir, target_speed, ground_accel, dt);
		if (keys[SDL_SCANCODE_SPACE]) {
			velocity.y = jump_power;
			is_grounded = false;
		}
	// Air
	} else {
		accelerate(wish_dir, max_air_speed, air_accel, dt);
	}
}

void PlayerV2::update_physics(float dt, const Chunk &chunk) {
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

void PlayerV2::toggle_mode() {
	if (mode == PlayerMode::Physical) {
		mode = PlayerMode::Spectator;
		position.y += 0.5f;
		velocity = Vec3{ 0.0f, 0.0f, 0.0f };
	} else {
		mode = PlayerMode::Physical;
	}
}

bool PlayerV2::check_collision(const Vec3 &test_pos, const Chunk &chunk) const {
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

void PlayerV2::apply_friction(float dt) {
	Vec3 flat_velocity = { velocity.x, 0.0f, velocity.z };
	float speed = flat_velocity.length();

	if (speed < 0.0001f) {
		velocity.x = 0.0f;
		velocity.z = 0.0f;
		return;
	}

	float drop = 0.0f;
	if (is_grounded) {
		float control = (speed < ground_accel) ? ground_accel : speed;
		drop = control * friction * dt;
	}

	float new_speed = speed - drop;
	if (new_speed < 0.0f) new_speed = 0.0f;

	new_speed /= speed;
	velocity.x *= new_speed;
	velocity.z *= new_speed;
}

void PlayerV2::accelerate(
	Vec3 wish_dir, float wish_speed, float accel, float dt
) {
	float current_speed = (velocity.x * wish_dir.x) + (velocity.z * wish_dir.z);
	float add_speed = wish_speed - current_speed;

	if (add_speed <= 0) return;

	float accel_speed = accel * wish_speed * dt; 

	if (accel_speed > add_speed) accel_speed = add_speed;
	velocity.x += accel_speed * wish_dir.x;
	velocity.z += accel_speed * wish_dir.z;
}

}  // namespace lili
