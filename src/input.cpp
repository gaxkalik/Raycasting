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
	if(key == 46)					//46 = '>' save map
	{
		if (action)
			keys.saveMap = true;
		else
			keys.saveMap = false;	
	}
	if(key == 48)					//48 = '0' load map
	{
		//std::cout << "000\n";
		if (action)
			keys.loadCustom = true;
		else
			keys.loadCustom = false;	
	}
		if(key == 32)					//48 = space shoot
	{
		if (action)
			keys.shoot = true;
		else
			keys.shoot = false;	
	}

	if (key == 256)
	{
		if (action)
			keys.esc = !keys.esc;
		
	}
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
			playerSpeed = playerStep;
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

void	raycast::collision(double &dirX, double &dirY) {
	std::pair<double, double> *pHitBox = pl->getHitBox();
	bool	x = false;
	bool	y = false;


	for (int i = 0; i < 4; ++i) {
		if ((*map)[pHitBox[i].second + dirY][pHitBox[i].first] != '0' && (*map)[pHitBox[i].second + dirY][pHitBox[i].first] != 'c')
			y = true;
		if ((*map)[pHitBox[i].second][pHitBox[i].first + dirX] != '0' && (*map)[pHitBox[i].second][pHitBox[i].first + dirX] != 'c')
			x = true;
	}
	if (!x)
		pl->setX(dirX);
	if (!y)
		pl->setY(dirY);
	pl->calculateHitBoxPosition();
}

void	raycast::playerInput(void) {

	double dirX = playerSpeed * sin(pAngle + M_PI_2);
	double dirY = playerSpeed * cos(pAngle + M_PI_2);

	double sDirX = playerSpeed * sin(pAngle);
	double sDirY = playerSpeed * cos(pAngle);

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
			playerSpeed = 2 * playerStep;
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
			{
				brush = '1';
				screenMessege = "wall 1";
			}
			else if (o->getName() == "buttonBrush2")
			{
				brush = '2';
				screenMessege = "wall 2";
			}
			else if (o->getName() == "buttonBrushP")
			{
				brush = 'p';
				screenMessege = "player";
			}
			else if (o->getName() == "buttonBrushD")
			{
				brush = 'd';
				screenMessege = "door";
			}	
			else if (o->getName() == "buttonBrushc")
			{
				brush = 'c';
				screenMessege = "coin";
			}	
			else if (o->getName() == "buttonBrush3")
			{
				brush = '3';
				screenMessege = "wall 3";
			}
			else if (o->getName() == "buttonBrush4")
			{
				brush = '4';
				screenMessege = "wall 4";
			}
			else if (o->getName() == "buttonBrushA")
			{
				brush = 'A';
				screenMessege = "crate 1";
			}
			else if (o->getName() == "buttonBrushB")
			{
				brush = 'B';
				screenMessege = "crate 2";
			}
			else if (o->getName() == "buttonBrushC")
			{
				brush = 'C';
				screenMessege = "crate 3";
			}
			else if (o->getName() == "buttonBrushCL")
			{
				brush = 'K';
				screenMessege = "delate";
			}
			else if (o->getName() == "Save")
			{
				saveMap();
				screenMessege = "SAVED";
			}
			else if (o->getName() == "Load")
			{
				keys.loadCustom=true;
			}

		}
	}

	if (keys.esc)
	{
		currScene = &(*scenes)[2];
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == 1) {
			glfwGetCursorPos(window, &cursorX, &cursorY);
			cursorX *= 2;
			cursorY *= 2;
			cursorY = screenBuffHeight - cursorY;
			obj	*o = cursorOnObj(cursorX, cursorY);
			if (o == nullptr)
				return;
			else if (o->getName() == "mainMenu") 
			{
				int	x = (cursorX - o->getX1()) / o->getTileWidth();
				int	y = (screenBuffHeight - cursorY - o->getY1()) / o->getTileHeight();
			}
			else if (o->getName() == "exit")
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else if (o->getName() == "editor")
			{
				keys.esc = false;
				keys.openMap = true;
			}
			else if (o->getName() == "Start")
			{
				keys.openMap = 0;
				keys.esc = 0;
				currScene = &(*scenes)[0];
			}
		}
	}
		
}