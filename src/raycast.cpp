#include "raycast.hpp"

extern	keyPressed keys;

raycast::raycast() {
	window = nullptr;
	map = nullptr;
	newMap = nullptr;
	pl = nullptr;
	mapWidth = 0;
	mapHeight = 0;
	screenWidth = 0;
	screenHeight = 0;
	std::cout << "[ Raycast default constructor called ]" << std::endl;
}

raycast::~raycast() {
	if (map)
		delete map;
	if (pl)
		delete pl;
	if (newMap)
		delete newMap;
	glfwTerminate();
	std::cout << "[ Raycast destructor called ]" << std::endl;
}

const int raycast::initGame(const char *filename) {
	pl = new player();
	if (mapParse(filename)) {
		std::cerr << "Failed to parse map" << std::endl;
		return 1;
	}
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return 1;
	}
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	screenWidth = mode->width;
	screenHeight = mode->height;
	std::cout << "Screen: " << screenWidth << ":" << screenHeight << std::endl;
	return 0;
}

const int	raycast::startGame(void) {
	window = glfwCreateWindow(screenWidth, screenHeight, "RayCasting", NULL, NULL);

	glfwGetFramebufferSize(window, &screenBuffWidth, &screenBuffHeight);
	glfwSetWindowUserPointer(window, this);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		playerInput();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}