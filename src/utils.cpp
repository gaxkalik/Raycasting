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