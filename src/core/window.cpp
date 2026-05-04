#include <stdexcept>
#include "core/window.hpp"

namespace lili {

Window::Window(const std::string &title, int width, int height) {
	window = SDL_CreateWindow(
		title.c_str(), width, height, SDL_WINDOW_RESIZABLE
	);
	if (!window)
		throw std::runtime_error(
			"Failed to create Window: " + std::string(SDL_GetError())
		);
}

Window::~Window() {
	if (window) SDL_DestroyWindow(window);
}

void Window::set_title(const std::string &title) {
	if (!SDL_SetWindowTitle(window, title.c_str()))
		throw std::runtime_error(
			"Failed to change window name: " + std::string(SDL_GetError())
		);
}

void Window::set_size(int width, int height) {
	if (!SDL_SetWindowSize(window, width, height))
		throw std::runtime_error(
			"Failed to change window size: " + std::string(SDL_GetError())
		);
}

void Window::set_resizable(bool activate) {
	if (!SDL_SetWindowResizable(window, activate))
		throw std::runtime_error(
			"Failed to change window to resizable: " +
			std::string(SDL_GetError())
		);
	resizable = activate;
}

void Window::set_borderless(bool activate) {
	if (!SDL_SetWindowBordered(window, !activate))
		throw std::runtime_error(
			"Failed to change window to borderless: " +
			std::string(SDL_GetError())
		);
	borderless = activate;
}

void Window::set_fullscreen(bool activate) {
	if (!SDL_SetWindowFullscreen(window, activate))
		throw std::runtime_error(
			"Failed to change window to fullscreen mode: " +
			std::string(SDL_GetError())
		);
	fullscreen = activate;
}

void Window::set_relative_mouse_mode(bool activate) {
	if (!SDL_SetWindowRelativeMouseMode(window, activate))
		throw std::runtime_error(
			"Failed to change window to relative mouse mode: " +
			std::string(SDL_GetError())
		);
}

const std::string Window::get_title() {
	return std::string(SDL_GetWindowTitle(window));
}

std::array<int, 2> Window::get_size() {
	int w, h = 0;
	if (!SDL_GetWindowSize(window, &w, &h))
		throw std::runtime_error(
			"Failed to get window size: " + std::string(SDL_GetError())
		);
	return {w, h};
}

bool Window::is_resizable() {
	return resizable;
}

bool Window::is_borderless() {
	return borderless;
}

bool Window::is_fullscreen() {
	return fullscreen;
}

bool Window::is_relative_mouse_mode() {
	return SDL_GetWindowRelativeMouseMode(window);
}

SDL_Window *Window::get_sdl_window() {
	return window;
}

}  // namespace lili
