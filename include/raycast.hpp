#ifndef RAYCAST_HPP
#define RAYCAST_HPP

// #include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <map>

#include "player.hpp"
#include "scene.hpp"

#define playerX pl->getX()
#define playerY pl->getY()
#define OBJ (it->second)

class	player;
class	scene;
class	obj;

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
		void		renderScene(scene &);
		void		renderBotton(const int &, const int &, const int &, const int &);
		void		renderMinimap(const int &, const int &, const int &, const int &);
		void		renderMapCreateToolField(const int &, const int &, const int &, const int &);
		void		newScene(void);
		void		addObjectToScene(scene &, const int &, const int &, const int &, const int &, const std::string &);
		void		addObjectToScene(scene &, const int &, const int &, const int &, const int &, const int &, const std::string &);
		void		floodFill(const int, const int, std::vector<std::string> &) const;
		obj	*cursorOnObj(const double &cursorX, const double &cursorY) const;
		static void	window_size_callback(GLFWwindow*, int, int);

	private:
		scene						*currScene;
		player						*pl;
		GLFWwindow					*window;
		std::vector<scene>			*scenes;
		std::vector<std::string>	*map;
		std::vector<std::string>	*newMap;
		int							mapWidth, mapHeight;
		int							screenWidth, screenHeight;
		int							screenBuffWidth, screenBuffHeight;
		double						cursorX, cursorY;
};

void		key_callback(GLFWwindow*, int, int, int, int);
const int	inRange(const double, const double, const double);

#endif