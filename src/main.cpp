#include <iostream>
#include <cstdlib>
#include "app.hpp"

int main(int argc, char *argv[]) {
	App app;

	try {
		if (argc > 1) app.run(argv[1]);
		else app.run();
	} catch (std::exception &e) {
		std::cerr << "ERR: " << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
