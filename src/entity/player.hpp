#pragma once

#include "geometry/vec3.hpp"
#include "world/map.hpp"
#include "render/scene/camera.hpp"
#include "core/keyboard.hpp"

namespace lili {

enum class PlayerMode {
	Physical,
	Spectator,
	Builder
};

class Player {
public:
	Player();
	~Player() = default;

	void set_position(const Vec3 &pos);

	Vec3 get_position() const;
	float get_build_range() const;
	PlayerMode get_mode() const;

	void process_keys(const Keyboard &keyboard, const Camera &camera);
	void update_physics(float dt, Map &map);
	void toggle_spectator();
	void toggle_builder();

private:
	Vec3 position;
	Vec3 velocity;
	Vec3 direction;

	bool jump_input;
	bool is_running;
	
	float walk_speed;
	float run_speed;
	float current_speed;
	float builder_speed;
	float spectator_speed;

	float ground_control;
	float air_control;

	float jump_power;
	float gravity;
	bool is_grounded;

	float width;
	float height;

	float build_range;

	PlayerMode mode;

	bool check_collision(const Vec3 &test_pos, Map &map) const;
};

}  // namespace lili
