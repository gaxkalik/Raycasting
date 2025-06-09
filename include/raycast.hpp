#ifndef RAYCAST_HPP
#define RAYCAST_HPP

// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "player.hpp"

#define playerX pl->getX()
#define playerY pl->getY()

class player;

class raycast
{
	public:
		raycast();
		~raycast();

		const int	startGame(void);
		const int	checkValidity(void) const;
		const int	initGame(const char *filename);
		const int	mapParse(const char *filename);
		void		playerInput(void);
		void		renderMinimap(void) const;
		void		floodFill(const int x, const int y, std::vector<std::string> &map) const;

	private:
		GLFWwindow					*window;
		int							screenWidth;
		int							screenHeight;
		std::vector<std::string>	*map;
		int							mapWidth;
		int							mapHeight;
		player						*pl;
};

#endif