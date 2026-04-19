#ifndef ENTITY_PLAYER_HPP
# define ENTITY_PLAYER_HPP

# include "math/vec3.hpp"
# include "map.hpp"

namespace lili {

enum class PlayerMode {
	Physical,
	Spectator
};

class Player {
public:
	Vec3 position = { 0.0f, 0.0f, 0.0f };
	Vec3 velocity = { 0.0f, 0.0f, 0.0f };
	
	float walk_speed = 5.0f;
	float run_speed = 7.0f;
	float fly_speed = 20.0f;

	float ground_control = 15.0f;
	float air_control = 4.0f;

	float jump_power = 8.0f;
	float gravity = -25.0f;
	bool is_grounded = false;

	float width = 0.6f;
	float height = 1.8f;

	PlayerMode mode = PlayerMode::Physical;

	void process_input(
		const bool *keys,
		const Vec3 &cam_front, const Vec3 &cam_right, const Vec3 &cam_up,
		float dt
	);
	void update_physics(float dt, Map &map);
	void toggle_mode();

private:
	bool check_collision(const Vec3 &test_pos, Map &map) const;
};

}  // namespace lili

#endif
