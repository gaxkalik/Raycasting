#include "raycast.hpp"

keyPressed keys;

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
	if (key == 83) {
		if (action)
			keys.moveDown = true;
		else
			keys.moveDown = false;
	}
	if (key == 87) {
		if (action)
			keys.moveUp = true;
		else
			keys.moveUp = false;
	}
	if (key == 77) {
		if (action)
			keys.openMap = !keys.openMap;
	}
	if (key == 256)
		if (action)
			keys.esc = true;
}

void	raycast::playerInput(void) {
	if (keys.openMap == false) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (keys.moveUp && (*map)[playerY - playerStep][playerX] != '1')
			pl->setY(-playerStep);
		if (keys.moveDown && (*map)[playerY + playerStep][playerX] != '1')
			pl->setY(playerStep);
		if (keys.moveLeft && (*map)[playerY][playerX - playerStep] != '1')
			pl->setX(-playerStep);
		if (keys.moveRight && (*map)[playerY][playerX + playerStep] != '1')
			pl->setX(playerStep);
	}
	else if (keys.openMap) {
		currScene = &(*scenes)[1];
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == 1) {
			glfwGetCursorPos(window, &cursorRealX, &cursorRealY);
			// std::cout << "Screen: " << screenWidth << ":" << screenHeight << std::endl;
			// // std::cout << "Tile: " << cursorX / tileWidth << ":" << cursorY / tileHeight << std::endl;
			int	x1 = 0, x2 = 0, y1 = 0, y2 = 0;
			int	viewPortSize = std::min(screenBuffWidth, screenBuffHeight) / 1.2;
			std::cout << "ScreenBuff: " << screenBuffWidth << ":" << screenBuffHeight << std::endl;
			tileWidth = viewPortSize / 32;
			tileHeight = tileWidth;
			// cursorMultX = std::ceil((double)screenBuffWidth / (double)viewPortSize);
			// cursorMultY = std::ceil((double)screenBuffHeight / (double)viewPortSize);
			
			// std::cout << "cursorMultX: " << cursorMultX << "\n";
			// std::cout << "cursorMultY: " << cursorMultY << "\n";
			std::cout << "CursorRealX: " << cursorRealX << ":" << cursorRealY << std::endl;
			// std::cout << "CursorX: " << cursorX << ":" << cursorY << std::endl;
			cursorX = cursorRealX * 2;
			cursorY = cursorRealY * 2;
			x1 = (screenBuffWidth - viewPortSize) / 2;
			x2 = x1 + viewPortSize;
			y1 = (screenBuffHeight - viewPortSize) / 2;
			y2 = y1 + viewPortSize;

			std::cout << "x1: " << x1 << "\n";
			std::cout << "x2: " << x2 << "\n";
			std::cout << "y1: " << y1 << "\n";
			std::cout << "y2: " << y2 << "\n";
			
			if (inRange(cursorX, x1, x2) && inRange(cursorY, y1, y2)) {
				std::cout << "In\n";
				if ((cursorX - x1) / tileWidth < 32 && (cursorY - y1) / tileHeight < 32)
				(*newMap)[(cursorY - y1) / tileHeight ][(cursorX - x1) / tileWidth ] = '1';
			}
		}
	}
	if (keys.esc)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}