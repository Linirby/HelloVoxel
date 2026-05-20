/// Window wrapper around SDL windowing.
#pragma once

#include <string>
#include <array>
#include <SDL3/SDL.h>

namespace lili {

class Window {
public:
	/// Create a window.
	Window();
	/// Destroy the window.
	~Window();

	/// Set the window title.
	void set_title(const std::string &title);
	/// Set the window size.
	void set_size(int width, int height);
	/// Enable or disable resizing.
	void set_resizable(bool activate);
	/// Enable or disable borderless mode.
	void set_borderless(bool activate);
	/// Enable or disable fullscreen mode.
	void set_fullscreen(bool activate);
	/// Enable or disable relative mouse mode.
	void set_relative_mouse_mode(bool activate);

	/// Get the current window title.
	const std::string get_title();
	/// Get the current window size.
	std::array<int, 2> get_size();
	/// Check if resizing is enabled.
	bool is_resizable();
	/// Check if borderless mode is enabled.
	bool is_borderless();
	/// Check if fullscreen mode is enabled.
	bool is_fullscreen();
	/// Check if relative mouse mode is enabled.
	bool is_relative_mouse_mode();

	/// Get the raw SDL window pointer.
	SDL_Window *get_sdl_window();

private:
	SDL_Window *window;
	bool resizable, borderless, fullscreen;
};

}  // namespace lili
