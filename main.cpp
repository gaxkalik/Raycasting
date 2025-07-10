#include "raycast.hpp"
#include <GLFW/glfw3.h>

void check_leaks() {
	system("leaks rc");
}

int main(int argc, char **argv) {
	// std::atexit(check_leaks);
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <map_file>" << std::endl;
		return 1;
	}
	raycast *game;

	try {
		game = new raycast();

		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
		if (game->initGame(argv[1])) {
			delete game;
			std::cerr << "Failed to initialize game" << std::endl;
			return 1;
		}
		if (game->startGame()) {
			delete game;
			std::cerr << "Failed to start game" << std::endl;
			return 1;
		}
		delete game;
	}
	catch (std::bad_alloc){
		 std::cerr << "Memory allocation failed: std::bad_alloc" << std::endl;
	}
	return 0;
}