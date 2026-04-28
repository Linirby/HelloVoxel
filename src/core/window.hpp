#ifndef CORE_WINDOW_HPP
# define CORE_WINDOW_HPP

# include <string>
# include <array>
# include <SDL3/SDL.h>

namespace lili {

struct WindowParameters {
	std::string &title;
	int width, height;
	bool resizable;
	bool borderless;
	bool fullscreen;
};

class Window {
public:
	Window(const std::string &title, int width, int height);
	~Window() = default;

	void set_title(const std::string &title);
	void set_size(int width, int height);
	void set_resizable(bool activate);
	void set_borderless(bool activate);
	void set_fullscreen(bool activate);
	void set_relative_mouse_mode(bool activate);

	const std::string get_title();
	std::array<int, 2> get_size();
	bool is_resizable();
	bool is_borderless();
	bool is_fullscreen();
	bool is_relative_mouse_mode();

private:
	SDL_Window *window = nullptr;
	bool resizable, borderless, fullscreen = false;
};

}  // namespace lili

#endif  // CORE_WINDOW_HPP
