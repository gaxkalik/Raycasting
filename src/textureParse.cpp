#include "raycast.hpp"

color	strgb(const std::string &line, const int &len) {
	color tmp;

	tmp.r = std::stoi(line.substr(5 + len, 2), nullptr, 16);
	tmp.g = std::stoi(line.substr(7 + len, 2), nullptr, 16);
	tmp.b = std::stoi(line.substr(9 + len, 2), nullptr, 16);

	return tmp;
}

int	raycast::loadRawTexture(const char* filename) {
	std::ifstream file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Faild to open '" << filename << "' texture file [invalid path]" << std::endl;
		return 1;
	}
	file.read(reinterpret_cast<char*>(txt), TEXTURE_SIZE);
	if (!file.gcount() == TEXTURE_SIZE) {
		std::cerr << "Faild to open '" << filename << "' texture file [dimension mismatch]" << std::endl;
		return 1;
	}

	return 0;
}

int	raycast::openTexture(const char *textureName) {
	std::ifstream	file;

	file.open(textureName);
	if (!file.is_open()) {
		std::cerr << "Faild to open '" << textureName << "' texture file [invalid path]" << std::endl;
		return 1;
	}
	int	s1 = 0;
	int	s2 = 0;
	texture txt;
	std::string line;

	std::getline(file, line);
	std::getline(file, line);
	std::getline(file, line);
	std::getline(file, line);

	std::vector<std::string> tmp = strSplit(line, ' ');
	txt.width = stoi(tmp[0]);
	txt.height = stoi(tmp[1]);
	txt.clCnt = stoi(tmp[2]);
	txt.pxLen = stoi(tmp[3]);
	// std::cout << txt.width << " " << txt.height << " " << txt.clCnt << " " << txt.pxLen << std::endl;
	for (int i = 0; i < txt.clCnt; ++i) {
		std::getline(file, line);
		txt.cl[line.substr(1, txt.pxLen)] = strgb(line, txt.pxLen);
	}
	std::getline(file, line);
	for (int i = 0; i < txt.height; ++i) {
		std::getline(file, line);
		txt.tx.push_back((line.substr(1, line.size() - 3)));
	}
	std::getline(file, line);
	txt.tx.push_back((line.substr(1, line.size() - 2)));
	textures.push_back(txt);
	file.close();
	return 0;
}