#ifndef ENTITY_PLAYERV2_HPP
# define ENTITY_PLAYERV2_HPP

# include "math/vec3.hpp"
# include "geometry/chunk.hpp"

namespace lili {

enum class PlayerMode {
	Physical,
	Spectator
};

class PlayerV2 {
public:
	Vec3 position = { 0.0f, 0.0f, 0.0f };
	Vec3 velocity = { 0.0f, 0.0f, 0.0f };
	
	float max_speed = 10.0f;
	float ground_accel = 25.0f;
	float friction = 12.0f;

	float max_air_speed = 1.5f;
	float air_accel = 500.0f;

	float jump_power = 9.0f;
	float gravity = -25.0f;

	float width = 0.6f;
	float standing_height = 1.8f;
	float crouching_height = 0.9;
	float height = standing_height;

	bool is_grounded = false;
	bool is_crouching = false;

	PlayerMode mode = PlayerMode::Physical;

	void process_input(
		const bool *keys,
		const Vec3 &cam_front, const Vec3 &cam_right, const Vec3 &cam_up,
		float dt
	);
	void update_physics(float dt, const Chunk &chunk);
	void toggle_mode();

private:
	bool check_collision(const Vec3 &test_pos, const Chunk &chunk) const;
	void apply_friction(float dt);
	void accelerate(
		Vec3 wish_dir, float wish_speed, float accel, float dt
	);
};

}  // namespace lili

#endif
