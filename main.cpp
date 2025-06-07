#include "raycast.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <map_file>" << std::endl;
		return 1;
	}
	raycast *game = new raycast();

	if (game->initGame(argv[1])) {
		delete game;
		std::cerr << "Failed to initialize game" << std::endl;
		return 1;
	}

	return 0;
}