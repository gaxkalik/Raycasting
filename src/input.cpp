#include "raycast.hpp"

keyPressed keys;

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == 87) {
		if (action)
			keys.moveUp = true;
		else
			keys.moveUp = false;
	}
	if (key == 83) {
		if (action)
			keys.moveDown = true;
		else
			keys.moveDown = false;
	}
	if (key == 65) {
		if (action)
			keys.moveLeft = true;
		else
			keys.moveLeft = false;
	}
	if (key == 68) {
		if (action)
			keys.moveRight = true;
		else
			keys.moveRight = false;
	}
	if (key == 256)
		if (action)
			keys.esc = true;
}

void	raycast::playerInput(void) {
	if (keys.moveUp && (*map)[playerY - playerStep][playerX] != '1')
		pl->setY(-playerStep);
	if (keys.moveDown && (*map)[playerY + playerStep][playerX] != '1')
		pl->setY(playerStep);
	if (keys.moveLeft && (*map)[playerY][playerX - playerStep] != '1')
		pl->setX(-playerStep);
	if (keys.moveRight && (*map)[playerY][playerX + playerStep] != '1')
		pl->setX(playerStep);
	if (keys.esc)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}