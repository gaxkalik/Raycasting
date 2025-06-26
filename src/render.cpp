#include "raycast.hpp"

#define BLACK 0,0,0					//0
#define WHITE 255,255,255			//1
#define WHITE_SHADED 200,200,200	//1
#define RED 255,0,0					//2
#define RED_SHADED 179,0,0			//2
#define GREEN 0,255,0				//3
#define GREEN_SHADED 0,179,0		//3
#define BLUE 0,0,255				//4
#define BLUE_SHADED 0,0,179			//4
#define GRAY 169,169,169			//5
#define GRAY_SHADED 118,118,118		//5
#define TRANSPARENT 255,255,255,0	//6
#define BLUE_SKY 135, 206, 235
#define GREEN_GRASS 17,124,19
#define BROWN 110, 38, 14
#define BROWN_SHADED 77,27,10

using std::cout;
using std::endl;

void raycast::window_size_callback(GLFWwindow* window, int width, int height) {
	raycast *rc = static_cast<raycast*>(glfwGetWindowUserPointer(window));
	rc->screenWidth = width;
	rc->screenHeight = height;
	glfwGetFramebufferSize(window, &rc->screenBuffWidth, &rc->screenBuffHeight);
}

void	raycast::addObjectToScene(scene &sc, const int &x, const int &y, const int &sizeX, const int &sizeY, const std::string &name) {
	sc.addObject(name, obj(x, y, sizeX, sizeY, name));
}

void	raycast::addObjectToScene(scene &sc, const int &x, const int &y, const int &sizeX, const int &sizeY, const int &tile, const std::string &name) {
	sc.addObject(name, obj(x, y, sizeX, sizeY, tile, name));
}

void	raycast::addObjectToScene(scene &sc, const int &x, const int &y, const int &sizeX, const int &sizeY, const int &tile, const std::string &name, const std::string &color) {
	sc.addObject(name, obj(x, y, sizeX, sizeY, tile, name, color));
}

void	raycast::renderBotton(const int &x1, const int &y1, const int &width, const int &height, const std::string &color) {
	glViewport(x1, y1, width, height);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 1, 1, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glEnable(GL_SCISSOR_TEST);
	glScissor(x1, y1, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	if (color == "white")
		glColor3f(1.0f, 1.0f, 1.0f);
	else if (color == "gray")
		glColor3f(0.3f, 0.3f, 0.3f);
	else if (color == "red")
		glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(1, 0);
	glVertex2f(1,1);
	glVertex2f(0,1);
	glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}

void	raycast::renderScene(scene &sc) {
	const std::map<std::string, obj> &objs = sc.getObjs();

	for (auto it = objs.begin(); it != objs.end(); ++it) {
		if (it->first == "minimap")
		{
			//rendTest(0, 0, screenBuffHeight, screenBuffHeight);
			renderGame(0,0,screenBuffWidth, screenBuffHeight);
			rendTest(0, 0, screenBuffHeight/4, screenBuffHeight/4);		//like minimap
			//renderMinimap(OBJ.getX1(), OBJ.getY1(), OBJ.getWidth(), OBJ.getHeight());
		}
		else if (it->first == "mapCreate")
			renderMapCreateToolField(OBJ.getX1(), OBJ.getY1(), OBJ.getWidth(), OBJ.getHeight());
		else if (it->first == "buttonBrush0")
			renderBotton(OBJ.getX1(), OBJ.getY1(), OBJ.getWidth(), OBJ.getHeight(), OBJ.getColor());
		else if (it->first == "buttonBrush1")
			renderBotton(OBJ.getX1(), OBJ.getY1(), OBJ.getWidth(), OBJ.getHeight(), OBJ.getColor());
		else if (it->first == "buttonBrushP")
			renderBotton(OBJ.getX1(), OBJ.getY1(), OBJ.getWidth(), OBJ.getHeight(), OBJ.getColor());
	}
}

void	raycast::renderMinimap(const int &x1, const int &y1, const int &width, const int &height) {
	double	viewPortCenterX = playerX;
	double	viewPortCenterY = playerY;

	if (!window || !map) {
		std::cerr << "Window or map not initialized." << std::endl;
		return;
	}
	glViewport(x1, y1, width, height);
	glEnable(GL_SCISSOR_TEST);
	glScissor(x1, y1, width, height);
	//glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 8, 0, 8, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	if (playerX + 4 > mapWidth)
		viewPortCenterX = mapWidth - 4;
	if (playerY + 4 > mapHeight)
		viewPortCenterY = mapHeight - 4;
	if (playerX - 4 < 0)
		viewPortCenterX = 4;
	if (playerY - 4 < 0)
		viewPortCenterY = 4;
	for (double y = -4; y < 4; y += playerStep) {
		for (double x = -4; x < 4; x += playerStep) {
			double	mapX = viewPortCenterX + x;
			double	mapY = viewPortCenterY - y;
			if (mapX >= 0 && mapY >=0 && mapX < mapWidth &&
					mapY < mapHeight &&
						(*map)[mapY][mapX] == '1') {
				glColor3f(0.3f, 0.3f, 0.3f); // Gray
			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White
			}
			glBegin(GL_QUADS);
			glVertex2f(x + 4, y + 4);
			glVertex2f(x + 4 + playerStep, y + 4);
			glVertex2f(x + 4 + playerStep, y + 4 + playerStep);
			glVertex2f(x + 4, y + 4 + playerStep);
			glEnd();
		}
	}
	viewPortCenterX = playerX - viewPortCenterX;
	viewPortCenterY = viewPortCenterY - playerY;
	//std::cout << "VIEW X - " << viewPortCenterX << " Y - " << viewPortCenterY << "\n";
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY);
	glVertex2f(4 + viewPortCenterX + 0.2f, 4 + viewPortCenterY);
	glVertex2f(4 + viewPortCenterX + 0.2f, 4 + viewPortCenterY + 0.2f);
	glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY + 0.2f);
	glEnd();

	double rayX = 400*cos(pAngle);
	double rayY = 400*sin(pAngle);

	using std::cout;




	// glBegin(GL_LINES);
	// glColor3f(0.0, 0.0, 0.6); 
	// glVertex2f(rayX, rayY);
	// glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY);
	// glEnd();



	int blockX = (int)playerX;
	int blockY = (int)playerY;

	double rPX = playerX - blockX;
	double rPY = playerY - blockY;				//?

	double lengthX;
	double lengthY;

	if ((pAngle >= 0 && pAngle <= M_PI_2) || (pAngle > 3 * M_PI_2 && pAngle <= 2 * M_PI))
	{
		lengthX = abs(1 - rPX);
		lengthY = tan(pAngle) * lengthX;
	}
	else
	{
		lengthX = -abs(rPX);
		lengthY = (tan(pAngle) * lengthX);
	}
	
	

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.1); 
	glVertex2f(playerX, 4 + viewPortCenterY);
	glVertex2f(playerX + lengthX, 4 + viewPortCenterY + lengthY);
	glEnd();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}

void raycast::rendTest(const int &x1, const int &y1, const int &width, const int &height) {
	double	viewPortCenterX = playerX;
	double	viewPortCenterY = playerY;

	if (!window || !map) {
		std::cerr << "Window or map not initialized." << std::endl;
		return;
	}
	glViewport(x1, y1, width, height);
	glEnable(GL_SCISSOR_TEST);
	glScissor(x1, y1, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, mapWidth, mapHeight, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	for (double y = 0; y < mapHeight; y += 1) {
		for (double x = 0; x < mapWidth; x += 1) {
						if ((*map)[y][x] == '1') {
				glColor3f(0.3f, 0.3f, 0.3f); // Gray
			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White
			}
			glBegin(GL_QUADS);
			glVertex2f(x + 0.01f, y + 0.01f);
			glVertex2f(x + 0.98f, y + 0.01f);
			glVertex2f(x + 0.98f, y + 0.98f);
			glVertex2f(x + 0.01f, y + 0.98f);
			glEnd();
		}
	}


	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(playerX - 0.1f, playerY - 0.1f);
	glVertex2f(playerX + 0.1f, playerY - 0.1f);
	glVertex2f(playerX + 0.1f, playerY + 0.1f);
	glVertex2f(playerX - 0.1f, playerY + 0.1f);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(playerX, playerY);
	glVertex2f(playerX + (cos(pAngle) * 1), playerY + (sin(pAngle) * 1));
	glEnd();

	double rAngle = pAngle + M_PI / 6;


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}

void raycast::renderGame(const int &x1, const int &y1, const int &width, const int &height)
{
	int				rayCnt = 180 * 2;
	int				txtRes = 16;
	const double	resolution = width / rayCnt;
	const double	maxWallHeight = height;

	if (!window || !map) 
	{
		std::cerr << "Window or map not initialized." << std::endl;
		return;
	}

	glViewport(x1, y1, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, rayCnt, height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_BLEND);											//to use glColor4ub(255, 0, 0, 100); transparent textures
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawBackground(rayCnt);


	double rAngle = pAngle - M_PI / 6;
	double posX = 0;

	for (int i = 0; i < rayCnt; ++i, rAngle += (0.0174533 / 6), ++posX)
	{
		char dir = 'a';
		double dist = getShortestRay(rAngle, dir);
		double	tmpX = ((mX - (int)mX) * txtRes);
		double	tmpY2 = ((mY - (int)mY) * txtRes);
		double wallHeiht = maxWallHeight / abs(cos(pAngle - rAngle) * dist * 0.55);
		
		
		double	posY = (maxWallHeight - wallHeiht)/2;
		double	tmpY = wallHeiht / txtRes;
		
		if (wallHeiht == maxWallHeight) {
			posY -= 100 / dist;
			tmpY += 50 / dist;
		}
		
		if(wallHeiht > maxWallHeight)
			wallHeiht = maxWallHeight;

		for (double y = 0; y < txtRes; ++y) 
		{
			determineTextureColor(hWall, dir, y, tmpX, tmpY2);


			glBegin(GL_QUADS);
			glVertex2f(posX,		posY + (y * tmpY));
			glVertex2f(posX + 1,	posY + (y * tmpY));
			glVertex2f(posX + 1,	posY + ((y + 1) * tmpY));
			glVertex2f(posX,		posY + ((y + 1) * tmpY));
			glEnd();
		}
			if (tmpX > txtRes)
				tmpX = 0;
		tmpX = ((mY - (int)mY) * txtRes);
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
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
			if ((*map)[mYV][mXV] == '1') {
				_mX = mXV;
				distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
				break;
			}
			mYV += offsetY;
			++mXV;
		}
		else{
			if (!((*map)[(int)mYV][(int)mXV-1] != '1')) {
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
	{
		horLengthY = 1 - rPY;
		horLengthX = horLengthY/tan(rayAngle);
	}
	if (rayAngle > M_PI)	
	{
		horLengthY = -rPY;
		horLengthX = horLengthY/tan(rayAngle);
	}

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
			offsetX = 0.5;
		else
			offsetX = 1/tan(rayAngle);

		if(rayAngle > 0 && rayAngle < M_PI)
		{
			if ((*map)[(int)mYH][(int)mXH] == '1') 
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
			if ((*map)[(int)mYH - 1][(int)mXH] == '1') 
			{
				_mY = mYH - 1;
				distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));
				break;
			}
			--mYH;
			mXH -= offsetX;
		}
		else
		mXH += offsetX;
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
	// cout << "distH\t" << distH <<"\t distV \t" << distV << "\tdist\t" << dist << "\t angle \t"<< pAngle<<"\t\n";
	// std::cout << "x - " << mX << " y - " << mY << " "<< asd<< std::endl;
	return dist;
}

void raycast::drawBackground(int rayCnt) const
{
	////////sky///////
	glColor3ub(BLUE_SKY);
	//glColor3f(0.1f, 0.708f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(rayCnt, 0);
	glVertex2f(rayCnt, screenBuffHeight / 2);
	glVertex2f(0, screenBuffHeight / 2);
	glEnd();
	//////grass/////////
	glColor3ub(GREEN_GRASS);
	glBegin(GL_QUADS);
	glVertex2f(0, screenBuffHeight / 2);
	glVertex2f(rayCnt, screenBuffHeight / 2);
	glVertex2f(rayCnt, screenBuffHeight);
	glVertex2f(0,screenBuffHeight);
	glEnd();
}

void raycast::determineTextureColor(std::vector<std::string> *texture, char dir, int h, int x, int y) const
{
	if(dir == 'h') 
	{
		if ((*texture)[h][x] == '0')
			glColor3ub(BLACK);
		if ((*texture)[h][x] == '1')
			glColor3ub(WHITE);
		if ((*texture)[h][x] == '2')
			glColor3ub(RED);
		if ((*texture)[h][x] == '3')
			glColor3ub(GREEN);
		if ((*texture)[h][x] == '4')
			glColor3ub(BLUE);
		if ((*texture)[h][x] == '5')
			glColor3ub(GRAY);
		if ((*texture)[h][x] == '6')
			glColor4ub(TRANSPARENT);
	} 
	else 
	{
		if ((*texture)[h][y] == '0')
			glColor3ub(BLACK);
		if ((*texture)[h][y] == '1')
			glColor3ub(WHITE_SHADED);
		if ((*texture)[h][y] == '2')
			glColor3ub(RED_SHADED);
		if ((*texture)[h][y] == '3')
			glColor3ub(GREEN_SHADED);
		if ((*texture)[h][y] == '4')
			glColor3ub(BLUE_SHADED);
		if ((*texture)[h][y] == '5')
			glColor3ub(GRAY_SHADED);
		if ((*texture)[h][y] == '6')
			glColor4ub(TRANSPARENT);
	}
}
