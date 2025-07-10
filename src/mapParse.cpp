#include "raycast.hpp"

using std::string;
using std::cout;
using std::cerr;

const int	raycast::mapParse(const char *filename) {
	std::ifstream	file;

	file.open(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		return 1;
	}

	std::string line;
	map = new std::vector<std::string>();
	newMap = new std::vector<std::string>();
	for (int i = 0; i < 32; ++i)
		newMap->push_back("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");

	while (std::getline(file, line)) {
		if (!line.compare("\0"))
			continue;
		if (line.compare(0, 2, "T:")) {
			map->push_back(line);
		}
		else if (line[0] != '\n'){
			std::vector<std::string>	tmp = strSplit(line, ' ');
		
			if (loadRawTexture(tmp[1][0], tmp[2].c_str()))
			{
				return 1;
			}
		}
		else {
			std::cerr << "Invalid file" << std::endl;
			return 1;
		}
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
			if ((*map)[y][x] == 'p') {
				pl->setX(x + 0.5);
				pl->setY(y + 0.5);
			}
		}
	}
	
	if(checkValidity())
	{
		return 1;
	}
	
	(*map)[playerY][playerX] = '0';

	std::cout << "Player coords x: " << pl->getX() << " y: " << pl->getY() << std::endl;
	std::cout<< "Map width: " << mapWidth << " Map height: " << mapHeight << std::endl;
	return 0;
}

void	raycast::floodFill(const int x, const int y, std::vector<std::string> &map) const
{
	if (x == mapWidth + 2 || x < 0 || y == mapHeight + 2 || y < 0 || 
		(map[y][x] >= '1' && map[y][x] <= '9') || (map[y][x] >= 'A' && map[y][x] <= 'Z'))
		{
			return;
		}
	map[y][x] = '1';
	floodFill(x + 1, y, map);
	floodFill(x - 1, y, map);
	floodFill(x, y + 1, map);
	floodFill(x, y - 1, map);
}

const int raycast::checkValidity() const
{
	bool foundPlayer = false;

	for (int line = 0; line < (*map).size(); ++line)
	{
		string currentLine = map->at(line);

		for (int calumn = 0; calumn < currentLine.size(); ++calumn)
		{
			char currentChar = currentLine[calumn];
			
			if (currentChar < '0' && currentChar > '9' && currentChar != 'p' && currentChar != '\n' && currentChar < 'A' && currentChar >'Z')
			{
				cerr << "Invalid element in map! \n";
				return 1; 
			}
			else if (currentChar == 'p')
			{
				if (foundPlayer)
				{
					cerr << "More than 1 palyer on the map \n";
					return 1;
				}
				else
					foundPlayer = true;
			}

		}
	}
		
	std::vector<std::string> *map = new std::vector<std::string>(this->map->begin(), this->map->end());
	string firstLineB = "";
	for(int i = 0; i < mapWidth; ++i)
		firstLineB += "B";

	map->insert(map->begin(), firstLineB);
	map->push_back(firstLineB);

	for (string &s : *map)
		s = "B" + s + "B";

	floodFill(playerX + 1, playerY + 1, *map);
	for (int i = 0; i < map->size(); ++i) {
		if ((*map)[i].find('0') != std::string::npos) {
			delete map;
			return 1;
		}
	}

	if (!foundPlayer)
	{
		cerr << "No player in the map \n";
		delete map;
		return 1;
	}
	
	if ((*map)[0].find('B') == std::string::npos)
	{
		cerr<< "Map is not playable\n";
		delete map;
	 	return 1;
	}
	
	cout << "Map is playable\n";
	delete map;
	return 0;

}