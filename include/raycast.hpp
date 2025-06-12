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

class	player;

class	raycast
{
	public:
		raycast();
		~raycast();

		const int	startGame(void);
		const int	checkValidity(void) const;
		const int	initGame(const char *);
		const int	mapParse(const char *);
		void		playerInput(void);
		void		renderMinimap(void) const;
		void		renderMapCreateToolField(void);
		void		asd(void);
		void		as(void);
		void		floodFill(const int, const int, std::vector<std::string> &) const;
		static void	window_size_callback(GLFWwindow*, int, int);

	private:
		GLFWwindow					*window;
		int							screenWidth;
		int							screenHeight;
		int							screenBuffWidth;
		int							screenBuffHeight;
		int							tileWidth;
		int							tileHeight;
		double						cursorX;
		double						cursorY;
		double						cursorRealX;
		double						cursorRealY;
		double						cursorMultX;
		double						cursorMultY;
		std::vector<std::string>	*map;
		std::vector<std::string>	*newMap;
		int							mapWidth;
		int							mapHeight;
		player						*pl;
};

void		key_callback(GLFWwindow*, int, int, int, int);
const int	inRange(const double, const double, const double);

#endif