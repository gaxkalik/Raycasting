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

	private:
		GLFWwindow					*window;
		std::vector<std::string>	*map;
		int							mapWidth;
		int							mapHeight;
		int							playerX;
		int							playerY;
};

#endif