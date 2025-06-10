#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raycast.hpp"

#define playerStep 0.02

struct keyPressed {
	bool	esc = false;
	bool	moveUp = false;
	bool	moveDown = false;
	bool	moveLeft = false;
	bool	moveRight = false;
};

class player
{
	public:
		player();
		~player();

		double	getX(void) const { return pX; }
		void	setX(double X) { pX += X; }
		double	getY(void) const {return pY; }
		void	setY(double Y) { pY += Y; }
	private:
		double	pX;
		double	pY;
};

#endif