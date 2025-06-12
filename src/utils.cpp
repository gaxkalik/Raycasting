#include "raycast.hpp"

const int	inRange(const double num, const double x1, const double x2) {
	if (num > x1 && num <x2)
		return (1);
	return (0);
}