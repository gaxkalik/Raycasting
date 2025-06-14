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

obj	*raycast::cursorOnObj(const double &cursorX, const double &cursorY) const {
	std::map<std::string, obj> &objs = currScene->getObjs();

	for (auto it = objs.begin(); it != objs.end(); ++it) {
		if (inRange(cursorX, OBJ.getX1(), OBJ.getX2()) && inRange(cursorY, OBJ.getY1(), OBJ.getY2())) {
			return &it->second;
		}
	}
	return nullptr;
}

void	raycast::playerInput(void) {
	if (keys.openMap == false) {
		currScene = &(*scenes)[0];
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
			glfwGetCursorPos(window, &cursorX, &cursorY);
			cursorX *= 2;
			cursorY *= 2;
			obj	*o = cursorOnObj(cursorX, cursorY);
			if (o == nullptr)
				return;
			else if (o->getName() == "mapCreate")
					if ((cursorX - o->getX1()) / o->getTileWidth() < 32 && (cursorY - o->getY1()) / o->getTileHeight() < 32)
						(*newMap)[(cursorY - o->getY1()) / o->getTileHeight()][(cursorX - o->getX1()) / o->getTileWidth()] = o->getBrush();
			else if (o->getName() == "buttonBrush1")
				o->setBrush('1');
			else if (o->getName() == "buttonBrush0")
				o->setBrush('0');
		}
	}
	if (keys.esc)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}