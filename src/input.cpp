#include "raycast.hpp"

keyPressed keys;

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	std::cout << key << "\n";
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
	if (key == 263)						//left
	{ 			
		if (action)
			keys.rotateLeft = true;
		else
			keys.rotateLeft = false;
	}
	if (key == 262)						//right
	{
		if (action)
			keys.rotateRight = true;
		else
			keys.rotateRight = false;
	}	

}

obj	*raycast::cursorOnObj(const double &cursorX, const double &cursorY) const {
	std::map<std::string, obj> &objs = currScene->getObjs();

	std::cout << "cursorX - " << cursorX << " CursorY - " << cursorY << std::endl;
	for (auto it = objs.begin(); it != objs.end(); ++it) {
		if (inRange(cursorX, OBJ.getX1(), OBJ.getX2()) && inRange(cursorY, OBJ.getY1(), OBJ.getY2())) {
			std::cout << it->second.getName() << std::endl;
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
		if(keys.rotateLeft)
			pl->setAngle(rotationSpeed);
		if(keys.rotateRight)
			pl->setAngle(-rotationSpeed);
	}
	else if (keys.openMap) {
		currScene = &(*scenes)[1];
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == 1) {
			glfwGetCursorPos(window, &cursorX, &cursorY);
			cursorX *= 2;
			cursorY *= 2;
			cursorY = screenBuffHeight - cursorY;
			obj	*o = cursorOnObj(cursorX, cursorY);
			if (o == nullptr)
				return;
			else if (o->getName() == "mapCreate") {
				int	x = (cursorX - o->getX1()) / o->getTileWidth();
				int	y = (screenBuffHeight - cursorY - o->getY1()) / o->getTileHeight();
				if (x < 32 && y < 32)
					(*newMap)[y][x] = brush;
			}
			else if (o->getName() == "buttonBrush1")
				brush = '1';
			else if (o->getName() == "buttonBrush0")
				brush = '0';
			else if (o->getName() == "buttonBrushP")
				brush = 'P';
		}
	}
	if (keys.esc)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}