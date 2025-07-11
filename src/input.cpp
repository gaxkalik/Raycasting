#include "raycast.hpp"

keyPressed keys;

void	key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
	//std::cout << key<<"\n";
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
	if (key == 69) {
		if (action)
			keys.use = true;
		else
			keys.use = false;
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
	if (key == 340)
	{
		if(action)
			keys.run = true;
		else
		{
			keys.run = false;
			playerStep = 0.05;
		}
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

bool	raycast::collision(double &dirX, double &dirY) {
	std::pair<double, double> *pHitBox = pl->getHitBox();

	// std::cout << "dirX " << dirX << " dirY " << dirY << "\n";
	for (int i = 0; i < 4; ++i) {
		// std::cout << "c - " << (*map)[pHitBox[i].second - dirY][pHitBox[i].first] << "\n";
		// std::cout << i << " " << pHitBox[i].second - dirY << " " << pHitBox[i].first << " " << pHitBox[i].second << " " << pHitBox[i].first + dirX <<"\n";
		if (!((*map)[pHitBox[i].second + dirY][pHitBox[i].first] == '0' && (*map)[pHitBox[i].second][pHitBox[i].first + dirX] == '0')) {
			return true;
		}
	}
	pl->setX(dirX);
	pl->setY(dirY);
	pl->calculateHitBoxPosition();
	return false;
}

void	raycast::playerInput(void) {

	
	double dirX = playerStep * sin(pAngle + M_PI_2);
	double dirY = playerStep * cos(pAngle + M_PI_2);

	double sDirX = playerStep * sin(pAngle);
	double sDirY = playerStep * cos(pAngle);

	// if (dirX < 0) dirX = 0;
	// if (dirY < 0) dirY = 0;

	if (keys.openMap == false) {
		currScene = &(*scenes)[0];
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		if (keys.use == true)
		{
			if 		((*map)[playerY][playerX - 1] == 'd')	(*map)[playerY][playerX - 1] = '0';
			else if ((*map)[playerY][playerX + 1] == 'd')	(*map)[playerY][playerX + 1] = '0';
			else if ((*map)[playerY - 1][playerX] == 'd')	(*map)[playerY - 1][playerX] = '0';
			else if ((*map)[playerY + 1][playerX] == 'd')	(*map)[playerY + 1][playerX] = '0';
		}
		if (keys.moveUp) {
			collision(dirX, dirY *= -1);
		}
		if (keys.moveDown) {
			collision(dirX *= -1, dirY);
		}
		if (keys.moveLeft) {
			collision(sDirX, sDirY *= -1);
		}
		if (keys.moveRight)
		{
			collision(sDirX *= -1, sDirY);
		}
		if(keys.rotateLeft)
		{
			pl->setAngle(-rotationSpeed);
		}
		if(keys.rotateRight)
			pl->setAngle(rotationSpeed);
		if (keys.run)
		{
			playerStep = 0.1;
		}
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
				brush = 'p';
		}
	}
	if (keys.esc)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}