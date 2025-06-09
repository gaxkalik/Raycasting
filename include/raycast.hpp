#ifndef RAYCAST_HPP
#define RAYCAST_HPP

// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

#include "player.hpp"

class player;

class raycast
{
	public:
		raycast();
		~raycast();

		const int	initGame(const char *filename);
		const int	mapParse(const char *filename);
		const int	startGame(void);
		void		renderMinimap(void) const;
		const int	checkValidity(void) const;
		void		floodFill(const int x, const int y, std::vector<std::string> &map) const;

	private:
		GLFWwindow					*window;
		int							screenWidth;
		int							screenHeight;
		std::vector<std::string>	*map;
		int							mapWidth;
		int							mapHeight;
		player						*pl;
		double						playerX;
		double						playerY;
};

#endif