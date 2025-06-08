#include "raycast.hpp"

raycast::raycast() {
	window = nullptr;
	map = nullptr;
	mapWidth = 0;
	mapHeight = 0;
	screenWidth = 0;
	screenHeight = 0;
	playerX = 0;
	playerY = 0;
	std::cout << "[ Raycast default constructor called ]" << std::endl;
}

raycast::~raycast() {
	if (map)
		delete map;
	glfwTerminate();
	std::cout << "[ Raycast destructor called ]" << std::endl;
}

const int raycast::initGame(const char *filename) {
	if (mapParse(filename)) {
		std::cerr << "Failed to parse map" << std::endl;
		return 1;
	}
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return 1;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	screenWidth = mode->width * 2;
	screenHeight = mode->height * 2;
	std::cout << "Screen: " << screenWidth << ":" << screenHeight << std::endl;
	// screenWidth = 500;
	// screenHeight = 1024;
	return 0;
}

const int	raycast::startGame(void) {
	window = glfwCreateWindow(screenWidth, screenHeight, "RayCasting", NULL, NULL);

	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return 1;
	}
	glfwMakeContextCurrent(window);
	while (!glfwWindowShouldClose(window)) {
		
		glClear(GL_COLOR_BUFFER_BIT);

		renderMinimap();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}