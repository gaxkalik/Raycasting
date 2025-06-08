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
				(*map)[y][x] = '0';
				playerX = x + 0.4;
				playerY = y + 0.4;
			}
		}
	}
	std::cout << "Player coords x: " << playerX << " y: " << playerY << std::endl;
	std::cout<< "Map width: " << mapWidth << " Map height: " << mapHeight << std::endl;
	return 0;
}

void	raycast::floodFill(const int x, const int y, std::vector<std::string> &map) const {
	if (x == mapWidth || x < 0 || y == mapHeight || y < 0 || map[y][x] == '1')
		return;
	map[y][x] = '1';
	floodFill(x + 1, y, map);
	floodFill(x - 1, y, map);
	floodFill(x, y + 1, map);
	floodFill(x, y - 1, map);
}