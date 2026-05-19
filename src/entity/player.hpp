/// Player controller and movement modes.
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
	/// Create a player controller.
	Player();
	/// Destroy the player controller.
	~Player() = default;

	/// Set the player world position.
	void set_position(const Vec3 &pos);
	/// Attach a camera to the player.
	void set_camera(Camera &camera);
	/// Set the currently selected block ID.
	void set_selected_block(uint16_t block);

	/// Get the current position.
	Vec3 get_position() const;
	/// Get the interpolated position for rendering.
	Vec3 get_interpolated_position(float alpha) const;
	/// Get the build range in blocks.
	float get_build_range() const;
	/// Get the selected block ID.
	uint16_t get_selected_block() const;
	/// Get the current movement mode.
	PlayerMode get_mode() const;

	/// Process keyboard input to update movement intent.
	void process_keys(const Keyboard &keyboard);
	/// Process mouse input for block interactions.
	void process_mouse(const Mouse &mouse, WorldRuntime *world);
	/// Update movement physics for the current frame.
	void update_physics(float dt, const Map &map);
	/// Toggle spectator mode.
	void toggle_spectator();
	/// Toggle builder mode.
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
