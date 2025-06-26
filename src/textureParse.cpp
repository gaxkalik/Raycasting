#include "raycast.hpp"

void	raycast::openTexture(std::string textureName) {
	std::ifstream	file;

	file.open(textureName);
	if (!file.is_open()) {
		std::cerr << "Failed to open texture file" << std::endl;
		return 1;
	}

	std::string line;
	std::vector<std::string> texture;

	while (std::getline(file, line)) {
		// texture.push_back(line);
		std::cout << line << std::endl;
	}
}