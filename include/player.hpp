#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "raycast.hpp"

struct keyPressed {
	bool moveUp = false;
	bool moveDown = false;
	bool moveLeft = false;
	bool moveRight = false;
};

class player
{
	public:
		player();
		player(double, double);
		double	getX(void) const;
		void	setX(double);
		double	getY(void) const;
		void	setY(double);
		~player();
	private:
		double	playerX;
		double	playerY;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif