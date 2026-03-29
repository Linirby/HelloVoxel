#include <iostream>
#include <cstdlib>
#include "app.hpp"

int main() {
	App app;

	try {
		app.run();
	} catch (std::exception e) {
		std::cerr << "ERR: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
