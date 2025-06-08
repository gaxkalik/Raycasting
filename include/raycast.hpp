#ifndef RAYCAST_HPP
#define RAYCAST_HPP

// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>


class raycast
{
	public:
		raycast();
		~raycast();

		const int	initGame(const char *filename);
		const int	mapParse(const char *filename);
		const int	startGame(void);
		void		renderMinimap(void) const;

	private:
		GLFWwindow					*window;
		int							screenWidth;
		int							screenHeight;
		std::vector<std::string>	*map;
		int							mapWidth;
		int							mapHeight;
		double						playerX;
		double						playerY;
};

#endif