#include "raycast.hpp"

color	strgb(const std::string &line, const int &len) {
	color tmp;

	tmp.r = std::stoi(line.substr(5 + len, 2), nullptr, 16);
	tmp.g = std::stoi(line.substr(7 + len, 2), nullptr, 16);
	tmp.b = std::stoi(line.substr(9 + len, 2), nullptr, 16);

	return tmp;
}

int	raycast::loadRawTexture(const char* filename) {
	unsigned char	***tmpTexture = new unsigned char **[64];

	for (int i = 0; i < 64; ++i) {
		tmpTexture[i] = new unsigned char * [64];
		for (int j = 0; j < 64; ++j) {
			tmpTexture[i][j] = new unsigned char [4];
			memset(tmpTexture[i][j], 0, 4 * sizeof(unsigned char));
		}
	}

	std::ifstream	file(filename, std::ios::binary);
	if (!file) {
		std::cerr << "Faild to open '" << filename << "' texture file [invalid path]" << std::endl;
		return 1;
	}
	for (int i = 0; i < 64; ++i) {
		for (int j = 0; j < 64; ++j) {
			if (!file.read(reinterpret_cast<char*>(tmpTexture[i][j]), 4))
				break;
		}
	}
	if (!file.gcount() == TEXTURE_SIZE) {
		std::cerr << "Faild to open '" << filename << "' texture file [dimension mismatch]" << std::endl;
		return 1;
	}
	allTextures[filename] = tmpTexture;
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
	texture tmpTexture;
	std::string line;

	std::getline(file, line);
	std::getline(file, line);
	std::getline(file, line);
	std::getline(file, line);

	std::vector<std::string> tmp = strSplit(line, ' ');
	tmpTexture.width = stoi(tmp[0]);
	tmpTexture.height = stoi(tmp[1]);
	tmpTexture.clCnt = stoi(tmp[2]);
	tmpTexture.pxLen = stoi(tmp[3]);
	for (int i = 0; i < tmpTexture.clCnt; ++i) {
		std::getline(file, line);
		tmpTexture.cl[line.substr(1, tmpTexture.pxLen)] = strgb(line, tmpTexture.pxLen);
	}
	std::getline(file, line);
	for (int i = 0; i < tmpTexture.height; ++i) {
		std::getline(file, line);
		tmpTexture.tx.push_back((line.substr(1, line.size() - 3)));
	}
	std::getline(file, line);
	tmpTexture.tx.push_back((line.substr(1, line.size() - 2)));
	textures.push_back(tmpTexture);
	file.close();
	return 0;
}