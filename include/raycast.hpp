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

inline int	screenWidth;
inline int	screenHeight;

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
		void		renderMinimapTopL(void) const;
		void		renderMinimapTopR(void) const;
		void		renderMinimapBottomL(void) const;
		void		renderMinimapBottomR(void) const;
		void		floodFill(const int, const int, std::vector<std::string> &) const;

	private:
		GLFWwindow					*window;
		std::vector<std::string>	*map;
		int							mapWidth;
		int							mapHeight;
		player						*pl;
};

void	window_size_callback(GLFWwindow*, int, int);
void	key_callback(GLFWwindow*, int, int, int, int);

#endif