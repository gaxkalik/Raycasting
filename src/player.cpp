#include "player.hpp"



player::player(): pX(0), pY(0), angle(/*3 **/ M_PI / 2), hitbox(nullptr){ hitbox = new std::pair<double, double>[4]; };

player::~player(){ delete[] hitbox; };

void	player::calculateHitBoxPosition(void) {
	hitbox[0] = {pX - 0.1, pY - 0.1};
	hitbox[1] = {pX + 0.1, pY - 0.1};
	hitbox[2] = {pX + 0.1, pY + 0.1};
	hitbox[3] = {pX - 0.1, pY + 0.1};
}