#include "raycast.hpp"

raycast::raycast() {
	window = nullptr;
	map = nullptr;
	mapWidth = 0;
	mapHeight = 0;
	playerX = 0;
	playerY = 0;
	std::cout << "[ Raycast default constructor called ]" << std::endl;
}

raycast::~raycast() {
	if (window) {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	if (map)
		delete map;
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
	window = glfwCreateWindow(mode->width, mode->height, "RayCasting", NULL, NULL);

	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return 1;
	}

	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		
		glBegin(GL_QUADS);
			glVertex2f(-0.1f, -0.5f);
			glVertex2f(0.5f, -0.5f);
			glVertex2f(0.5f, 0.5f);
			glVertex2f(-0.5f, 0.5f);
		glEnd();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}