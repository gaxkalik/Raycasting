#include "raycast.hpp"

keyPressed keys;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == 87) {
		if (action)
			keys.moveUp = true;
		else
			keys.moveUp = false;
	}
	else if (key == 83) {
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
	else if (key == 68) {
		if (action)
			keys.moveRight = true;
		else
			keys.moveRight = false;
	}
}
