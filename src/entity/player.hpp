#pragma once

#include "geometry/vec3.hpp"
#include "world/map.hpp"
#include "render/scene/camera.hpp"
#include "core/keyboard.hpp"
#include "core/mouse.hpp"
#include "runtime/world_runtime.hpp"

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
	void set_camera(Camera &camera);
	void set_selected_block(uint16_t block);

	Vec3 get_position() const;
	Vec3 get_interpolated_position(float alpha) const;
	float get_build_range() const;
	uint16_t get_selected_block() const;
	PlayerMode get_mode() const;

	void process_keys(const Keyboard &keyboard);
	void process_mouse(const Mouse &mouse, WorldRuntime *world);
	void update_physics(float dt, const Map &map);
	void toggle_spectator();
	void toggle_builder();

private:
	Camera *camera;

	Vec3 previous_position;
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
	uint8_t selected_block;

	PlayerMode mode;

	bool check_collision(const Vec3 &test_pos, const Map &map) const;
};

}  // namespace lili
