#include "raycast.hpp"

double playerStep = 0.05;

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
	brush = '0';
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

void	raycast::addBottonsToScene(void) {
	std::map<std::string, obj>	&tmp = (*scenes)[1].getObjs();

	auto it = tmp.find("mapCreate");
	int tileWidth = it->second.getTileWidth();
	int tileHegiht = it->second.getTileHeight();
	int x = it->second.getX1();
	int y = it->second.getY1() - tileHegiht;

	addObjectToScene((*scenes)[1], x, y, tileWidth, tileHegiht, 1, "buttonBrush1", "gray");
	addObjectToScene((*scenes)[1], x + tileWidth, y, tileWidth, tileHegiht, 1, "buttonBrush0", "white");
	addObjectToScene((*scenes)[1], x + tileWidth * 2, y, tileWidth, tileHegiht, 1, "buttonBrushP", "red");
}

void glColorARGB(uint32_t argb) {
	GLubyte r = (argb >> 16)	& 0xFF;
	GLubyte g = (argb >> 8)		& 0xFF;
	GLubyte b = (argb)			& 0xFF;
	glColor3ub(r, g, b);
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
	addObjectToScene((*scenes)[0], 0, 0, screenBuffWidth, screenBuffHeight, "MainGame");
	addObjectToScene((*scenes)[0], 0, 0, size / 4, size / 4, "minimap");


	newScene();
	addObjectToScene((*scenes)[1], (screenBuffWidth - size / 1.2) / 2,\
	(screenBuffHeight - size / 1.2) / 2, size / 1.2, size / 1.2, 32, "mapCreate");
	addBottonsToScene();
	if (openTexture("textures/CASTLEBRICKS.xpm"))
		return 1;
	if (loadRawTexture("textures/output.raw"))
		return 1;

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

const double raycast::getVerticalRay(double rayAngle, double &_mX, double &_mY) const
{
	double vertLengthX = 0;
	double vertLengthY = 0;
	double rPX = playerX - (int)playerX;

	if (rayAngle > 2 * M_PI)
		rayAngle = rayAngle - 360 * 0.0174533;
	if (rayAngle < 0)
		rayAngle += 2 * M_PI;
	if ((rayAngle >= 0 && rayAngle < M_PI_2) || (rayAngle > 3 * M_PI_2 && rayAngle <= 2 * M_PI))			//right
	{
		vertLengthX = abs(1 - rPX);
		vertLengthY = tan(rayAngle) * vertLengthX;
	}
	else if ((rayAngle == M_PI_2 || rayAngle == 3 * M_PI_2)) {
		vertLengthX = 0;
		vertLengthY = 0;
	}
	else																							//left
	{
		vertLengthX = -abs(rPX);
		vertLengthY = (tan(rayAngle) * vertLengthX);
	}

	double mXV = playerX + vertLengthX;
	double mYV = playerY + vertLengthY;
	double offsetY = tan(rayAngle);
	double distV = 100;
	
	while (mXV >= 0 && mXV < mapWidth && mYV >= 0 && mYV < mapHeight) {
		if ((rayAngle >= 0 && rayAngle <= M_PI_2) || (rayAngle > 3 * M_PI_2 && rayAngle <= 2 * M_PI))
		{
			if ((*map)[mYV][mXV] >= '1' && (*map)[mYV][mXV] <= '9') {
				_mX = mXV;
				distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
				break;
			}
			mYV += offsetY;
			++mXV;
		}
		else{
			if ((*map)[(int)mYV][(int)mXV-1] >= '1' && (*map)[(int)mYV][(int)mXV-1] <= '9') {
				_mX = mXV - 1;
				distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
				break;
			}
			--mXV;
			mYV -= offsetY;
		}
	}
	_mY = mYV;
	return distV;
}

const double  raycast::getHorizontalRay(double rayAngle, double &_mX, double &_mY) const
{
	double rPX = playerX - (int)playerX;
	double rPY = playerY - (int)playerY;

	double horLengthX = rPX;
	double horLengthY = rPY;
	double offsetX = 0;

	if (rayAngle > 2 * M_PI || rayAngle > 7)
		rayAngle = rayAngle - 360 * 0.0174533;
	if (rayAngle < 0)
		rayAngle += 2 * M_PI;
	
	if(rayAngle < M_PI)
		horLengthY = 1 - rPY;
	if (rayAngle > M_PI)
		horLengthY = -rPY;
	if (rayAngle != 3 * M_PI_2 && rayAngle != M_PI_2 && rayAngle != M_PI && rayAngle != 0){
		horLengthX = horLengthY/tan(rayAngle);
	}
	else
	{
		horLengthX = 0;
	}
	// std::cout<< horLengthX << " " << horLengthY << std::endl;
	
	double mXH = playerX + horLengthX;
	double mYH = playerY + horLengthY;

	double distH = 100;
	if (mXH > mapWidth) {
		mXH = mapWidth;
		return distH;
	}
	
	while (mXH >= 0 && mXH < mapWidth && mYH >= 0 && mYH < mapHeight)
	{
		if(rayAngle == 0 || rayAngle == M_PI || rayAngle == M_PI/2 || rayAngle == 3*M_PI/2)
			offsetX = 0;
		else
			offsetX = 1/tan(rayAngle);

		if(rayAngle > 0 && rayAngle < M_PI)
		{
			if ((*map)[(int)mYH][(int)mXH] >= '1' && (*map)[(int)mYH][(int)mXH] <= '9') 
			{
				_mY = mYH;
				distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));			
				break;
			}
			++mYH;
			mXH += offsetX;
		}
		else if ((rayAngle >= M_PI && rayAngle <= 3 * M_PI_2) || (rayAngle >= 3 * M_PI_2 && rayAngle <= 2 * M_PI))
		{
			if ((*map)[(int)mYH - 1][(int)mXH] >= '1' && (*map)[(int)mYH - 1][(int)mXH] <= '9') 
			{
				_mY = mYH - 1;
				distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));
				break;
			}
			--mYH;
			mXH -= offsetX;
		}
		else
			break;
	}
	_mX = mXH;
	return distH;
}

const double raycast::getShortestRay(double rayAngle, char &dir)
{
	double	xV, yV, xH, yH;
	double	distV = getVerticalRay(rayAngle, xV, yV);
	double	distH = getHorizontalRay(rayAngle, xH, yH);
	double	dist = 0;

	if (distV <= distH) {
		dist = distV;
		mX = xV;
		mY = yV;
	} else {
		dist = distH;
		mX = xH;
		mY = yH;
	}

	dir = distV <= distH ? 'v' : 'h';
	return dist;
}