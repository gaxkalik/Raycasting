#include "raycast.hpp"

extern	keyPressed keys;

raycast::raycast() {
	window = nullptr;
	map = nullptr;
	newMap = nullptr;
	pl = nullptr;
	scenes = nullptr;
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
	if (scenes)
		delete scenes;
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
	window = glfwCreateWindow(screenWidth, screenHeight, "RayCasting", NULL, NULL);
	glfwGetFramebufferSize(window, &screenBuffWidth, &screenBuffHeight);
	std::cout << "Screen: " << screenWidth << ":" << screenHeight << std::endl;

	int	size = std::min(screenBuffWidth, screenBuffHeight);
	scenes = new std::vector<scene>();

	newScene();
	addObjectToScene((*scenes)[0], 0, 0, size / 4, size / 4, "minimap");

	newScene();
	addObjectToScene((*scenes)[1], 0, 0, 256, 256, "buttonBrush1");
	addObjectToScene((*scenes)[1], (screenBuffWidth - size / 1.2) / 2,\
	(screenBuffHeight - size / 1.2) / 2, size / 1.2, size / 1.2, 32, "mapCreate");
	return 0;
}

void	raycast::newScene(void) {
	scenes->emplace_back();
}

const int	raycast::startGame(void) {
	glfwSetWindowUserPointer(window, this);
	if (!window) {
		glfwTerminate();
		std::cerr << "Failed to create GLFW window" << std::endl;
		return 1;
	}
	currScene = &(*scenes)[0];
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		playerInput();
		renderScene(*currScene);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
}