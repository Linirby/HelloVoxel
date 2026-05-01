#ifndef CORE_EVENT_HPP
# define CORE_EVENT_HPP

# include <SDL3/SDL_events.h>
# include <array>

namespace lili {

class Event {
public:
	struct {
		float x, y = 0.0f;
		float motion_xrel, motion_yrel = 0.0f;
	} mouse;

	bool process();
	bool quit_button();

	bool is_key_pressed(int key);
	bool is_mouse_pressed(int button);

	const bool *get_keyboard_state();
	std::array<float, 2> get_mouse_pos();

private:
	SDL_Event event;

    const bool *keyboard_state = nullptr;
	SDL_MouseButtonFlags mouse_state;
};

}  // namespace lili

#endif  // CORE_EVENT_HPP
