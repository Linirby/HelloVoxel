#include "entity/player.hpp"

#include <SDL3/SDL.h>
#include <cmath>

#include "physics/collision.hpp"
#include "world/block.hpp"

namespace lili {

Player::Player() {
	camera = nullptr;

	previous_position = {};
	position = {};
	velocity = {};
	direction = {};

	jump_input = false;
	is_running = false;
	
	walk_speed = 5.0f;
	run_speed = 7.0f;
	current_speed = 0.0f;
	builder_speed = 8.0f;
	spectator_speed = 15.0f;

	ground_control = 15.0f;
	air_control = 4.0f;

	jump_power = 8.0f;
	gravity = -25.0f;
	is_grounded = false;

	width = 0.8f;
	height = 1.8f;

	build_range = 12.0f;
	selected_block = 1;

	mode = PlayerMode::Physical;
}

void Player::set_position(const Vec3 &pos) {
	position = pos;
}

void Player::set_camera(Camera &camera) {
	this->camera = &camera;
}

void Player::set_selected_block(uint16_t block) {
	selected_block = block;
}

Vec3 Player::get_position() const {
	return position;
}

Vec3 Player::get_interpolated_position(float alpha) const {
	return previous_position.lerp(position, alpha);
}

float Player::get_build_range() const {
	return build_range;
}

uint16_t Player::get_selected_block() const {
	return selected_block;
}

PlayerMode Player::get_mode() const {
	return mode;
}

void Player::process_keys(const Keyboard &keyboard) {
	direction = {};
	is_running = false;
	jump_input = false;
	if (camera == nullptr) return;

	if (mode == PlayerMode::Spectator) {
		if (keyboard.held(SDL_SCANCODE_W)) direction += camera->front;
		if (keyboard.held(SDL_SCANCODE_S)) direction -= camera->front;
		if (keyboard.held(SDL_SCANCODE_D)) direction += camera->right;
		if (keyboard.held(SDL_SCANCODE_A)) direction -= camera->right;
		if (keyboard.held(SDL_SCANCODE_SPACE)) direction += camera->up;
		if (keyboard.held(SDL_SCANCODE_LSHIFT)) direction -= camera->up;
		return;
	}

	if (mode == PlayerMode::Builder) {
		Vec3 front = Vec3{
			camera->front.x, 0.0f, camera->front.z
		}.normalized();
		Vec3 right = Vec3{
			camera->right.x, 0.0f, camera->right.z
		}.normalized();
		Vec3 up = Vec3{ 0.0f, camera->up.y, 0.0f }.normalized();

		if (keyboard.held(SDL_SCANCODE_W)) direction += front;
		if (keyboard.held(SDL_SCANCODE_S)) direction -= front;
		if (keyboard.held(SDL_SCANCODE_D)) direction += right;
		if (keyboard.held(SDL_SCANCODE_A)) direction -= right;
		if (keyboard.held(SDL_SCANCODE_SPACE)) direction += up;
		if (keyboard.held(SDL_SCANCODE_LSHIFT)) direction -= up;
		return;
	}

	Vec3 flat_front = Vec3{
		camera->front.x, 0.0f, camera->front.z
	}.normalized();
	Vec3 flat_right = Vec3{
		camera->right.x, 0.0f, camera->right.z
	}.normalized();

	if (keyboard.held(SDL_SCANCODE_W)) direction += flat_front;
	if (keyboard.held(SDL_SCANCODE_S)) direction -= flat_front;
	if (keyboard.held(SDL_SCANCODE_D)) direction += flat_right;
	if (keyboard.held(SDL_SCANCODE_A)) direction -= flat_right;
	direction = (Vec3){ direction.x, 0.0f, direction.z }.normalized();

	is_running = (
		keyboard.held(SDL_SCANCODE_LSHIFT) && keyboard.held(SDL_SCANCODE_W)
	);
	jump_input = keyboard.held(SDL_SCANCODE_SPACE);
}

void Player::process_mouse(const Mouse &mouse, WorldRuntime *world) {
	BlockRegistry &block_registry = BlockRegistry::get();

	if (mouse.pressed(lili::MouseButton::LEFT)) {
		RaycastResult raycast = lili::raycast_voxel(
			camera->position,
			camera->front,
			get_build_range(),
			world->get_map()
		);
		if (raycast.hit) {
			world->remove_block({
				static_cast<float>(raycast.hit_x),
				static_cast<float>(raycast.hit_y),
				static_cast<float>(raycast.hit_z)
			});
		}
	}
	else if (mouse.pressed(lili::MouseButton::RIGHT)) {
		RaycastResult raycast = lili::raycast_voxel(
			camera->position,
			camera->front,
			get_build_range(),
			world->get_map()
		);
		if (raycast.hit) {
			world->add_block(selected_block, {
				static_cast<float>(raycast.adj_x),
				static_cast<float>(raycast.adj_y),
				static_cast<float>(raycast.adj_z)
			});
		}
	}
}

void Player::update_physics(float dt, const Map &map) {
	previous_position = position;

	if (mode == PlayerMode::Spectator) {
		position += direction * spectator_speed * dt;
		return;
	}

	if (mode == PlayerMode::Builder) {
		position += direction * builder_speed * dt;
		return;
	}

	float speed = is_running ? run_speed : walk_speed;
	float control = is_grounded ? ground_control : air_control;
	velocity.x += (direction.x * speed - velocity.x) * control * dt;
	velocity.z += (direction.z * speed - velocity.z) * control * dt;

	if (jump_input && is_grounded) {
		velocity.y = jump_power;
		is_grounded = false;
	}
	velocity.y += gravity * dt;
	
	Vec3 next_x = position;
	next_x.x += velocity.x * dt;
	if (!check_collision(next_x, map))
		position.x = next_x.x;
	else
		velocity.x = 0.0f;

	Vec3 next_y = position;
	next_y.y += velocity.y * dt;
	is_grounded = false;
	if (!check_collision(next_y, map))
		position.y = next_y.y;
	else {
		if (velocity.y < 0.0f) {
			is_grounded = true;
			position.y = std::floor(position.y);
		}
		velocity.y = 0.0f;
	}

	Vec3 next_z = position;
	next_z.z += velocity.z * dt;
	if (!check_collision(next_z, map))
		position.z = next_z.z;
	else
		velocity.z = 0.0f;
}

void Player::toggle_spectator() {
	if (mode != PlayerMode::Spectator) {
		mode = PlayerMode::Spectator;
		position.y += 0.5f;
		velocity = Vec3{ 0.0f, 0.0f, 0.0f };
	} else
		mode = PlayerMode::Physical;
}

void Player::toggle_builder() {
	if (mode != PlayerMode::Builder) {
		mode = PlayerMode::Builder;
		velocity = Vec3{ 0.0f, 0.0f, 0.0f };
	} else
		mode = PlayerMode::Physical;
}

bool Player::check_collision(const Vec3 &test_pos, const Map &map) const {
	float pad = 0.05f;

	Vec3 min = {
		test_pos.x - (width / 2.0f) + pad,
		test_pos.y,
		test_pos.z - (width / 2.0f) + pad
	};
	Vec3 max = {
		test_pos.x + (width / 2.0f) - pad,
		test_pos.y + height,
		test_pos.z + (width / 2.0f) - pad
	};

	for (int x = std::floor(min.x); x <= std::floor(max.x); ++x) {
		for (int y = std::floor(min.y); y <= std::floor(max.y); ++y) {
			for (int z = std::floor(min.z); z <= std::floor(max.z); ++z) {
				if (map.get_block_global(x, y, z) != 0)
					return true;
			}
		}
	}

	return false;
}

}  // namespace lili
