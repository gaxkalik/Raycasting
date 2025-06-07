#include "raycast.hpp"

const int	raycast::mapParse(const char *filename) {
	std::ifstream	file;

	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		return 1;
	}

	std::string line;
	map = new std::vector<std::string>();

	while (std::getline(file, line)) {
		map->push_back(line);
	}
	file.close();
	mapHeight = map->size();
	for (int i = 0; i < mapHeight; ++i) {
		if ((*map)[i].size() > mapWidth) {
			mapWidth = (*map)[i].size();
		}
	}

	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			if ((*map)[y][x] == 'P') {
				playerX = x;
				playerY = y;
			}
		}
	}
	std::cout << "Player coords x: " << playerX << ", y: " << playerY << std::endl;
	std::cout<< "Map width: " << mapWidth << ", Map height: " << mapHeight << std::endl;
	return 0;
}