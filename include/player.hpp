#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raycast.hpp"

#define playerStep 0.025
#define rotationSpeed 0.03

struct keyPressed {
	bool	esc = false;
	bool	moveUp = false;
	bool	moveDown = false;
	bool	moveLeft = false;
	bool	moveRight = false;
	bool	openMap = false;
	bool	rotateLeft = false;
	bool	rotateRight = false;
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
		double	getAngle(void) const { return angle; }
		void	setAngle(double angle)
		{
			this->angle += angle;
			if (this->angle < 0)
			{
				this->angle = 2 * M_PI;
			}
			if (this->angle > 2 * M_PI)
			{
				this->angle = 0;
			}
		}
	private:
		double	pX;
		double	pY;
		double angle;
};

#endif