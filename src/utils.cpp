#include "raycast.hpp"

const int	inRange(const double num, const double x1, const double x2) {
	if (num > x1 && num <x2)
		return (1);
	return (0);
}

std::vector<std::string> strSplit(const std::string &str, const char c) {
	int	s1, s2;
	std::vector<std::string> tmp;

	s1 = 1;
	while (1) {
		s2 = str.find(c, s1);
		if (s2 != std::string::npos) {
			tmp.push_back(str.substr(s1, s2 - s1));
			s1 = s2 + 1;
		}
		else
			break;
	}
	return (tmp);
}

int*** xpmToRaw(texture &tx)
{
	int*** rawTexture = new int**[tx.height];

	for (int i = 0; i < tx.height; ++i) {
		rawTexture[i] = new int*[tx.width];
		for (int j = 0; j < tx.width; ++j)
			rawTexture[i][j] = new int[4];
	}

	for(int line = 0; line < tx.height; ++line) {
		for(int calumn = 0; calumn < tx.width; ++calumn) {
			std::string key =  "";
			key += (char)tx.tx[line][calumn];
			rawTexture[line][calumn][0] = tx.cl[key].r;
			rawTexture[line][calumn][1] = tx.cl[key].g;
			rawTexture[line][calumn][2] = tx.cl[key].b;
			rawTexture[line][calumn][3] = tx.cl[key].a;
		}
	}

	return rawTexture;
}