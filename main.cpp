#include "raycast.hpp"
#include <GLFW/glfw3.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <map_file>" << std::endl;
		return 1;
	}
	raycast *game = new raycast();

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
	// system("leaks rc");
	return 0;
}