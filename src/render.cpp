#include "raycast.hpp"


#define BLUE_SKY 135, 206, 235
#define GREEN_GRASS 17,124,19

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
			// drawBackground(0);
			renderGame(0, 0, screenBuffWidth, screenBuffHeight);
			rendTest(OBJ.getX1(), 1000, screenBuffHeight / 4, screenBuffHeight / 4);
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
	// glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
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
	for (double y = -4; y < 4; y += 0.05) {
		for (double x = -4; x < 4; x += playerStep) {
			double	mapX = viewPortCenterX + x;
			double	mapY = viewPortCenterY - y;
			if (mapX >= 0 && mapY >=0 && mapX < mapWidth && mapY < mapHeight && (*map)[mapY][mapX] >= '1' && (*map)[mapY][mapX] <='9')
			{
				glColor3f(0.3f, 0.3f, 0.3f); // Gray
			} 
			else {
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
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY);
	glVertex2f(4 + viewPortCenterX + 0.2f, 4 + viewPortCenterY);
	glVertex2f(4 + viewPortCenterX + 0.2f, 4 + viewPortCenterY + 0.2f);
	glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY + 0.2f);
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
	// glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
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
	double pStep = 0.05;
	// std::cout << "____________________________\n";
	for (double y = -4; y < 4; y += pStep) {
		for (double x = -4; x < 4; x += pStep) {
			int	mapX = viewPortCenterX + x;
			int	mapY = viewPortCenterY - y;
			if (mapX >= 0 && mapY >=0 && mapX < mapWidth && mapY < mapHeight && (*map)[mapY][mapX] >= '1' && (*map)[mapY][mapX] <='9') {
				glColor3f(0.3f, 0.3f, 0.3f); // Gray
			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White
			}

			glBegin(GL_QUADS);
			glVertex2f(x + 4 + 0.1f, y + 4 + 0.1f);
			glVertex2f(x + 4 - pStep - 0.1f, 0.1f + y + 4);
			glVertex2f(x + 4 - pStep - 0.1f, y + 4 - pStep - 0.1f);
			glVertex2f(x + 4 + 0.1f, y + 4 - pStep- 0.1f);
			glEnd();
		}
	}
	viewPortCenterX = playerX - viewPortCenterX;
	viewPortCenterY = viewPortCenterY - playerY;
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY);
	glVertex2f(4 + viewPortCenterX + 0.2f, 4 + viewPortCenterY);
	glVertex2f(4 + viewPortCenterX + 0.2f, 4 + viewPortCenterY + 0.2f);
	glVertex2f(4 + viewPortCenterX, 4 + viewPortCenterY + 0.2f);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(4 + viewPortCenterX + 0.1f, 4 + viewPortCenterY + 0.1f);
	glVertex2f(4 + viewPortCenterX + 0.1f + (cos(pAngle) * 0.5), 4 + 0.1f + viewPortCenterY + (-sin(pAngle) * 0.5));
	glEnd();
	// for (double y = mapWidth - 1; y >= 0; y -= 1) {
	// 	for (double x = 0; x < mapWidth; x += 1) {
			
	// 		if ((*map)[y][x] == '1')
	// 			glColor3f(0.3f, 0.3f, 0.3f); // Gray
	// 		else
	// 			glColor3f(1.0f, 1.0f, 1.0f); // White

	// 		glBegin(GL_QUADS);
	// 		glVertex2f(x + 0.1f, y + 0.1f);
	// 		glVertex2f(x + 0.9f, y + 0.1f);
	// 		glVertex2f(x + 0.9f, y + 0.9f);
	// 		glVertex2f(x + 0.1f, y + 0.9f);
	// 		glEnd();
	// 	}
	// }

	// viewPortCenterX = playerX - viewPortCenterX;
	// viewPortCenterY = viewPortCenterY - playerY;
	// glColor3f(1.0f, 0.0f, 0.0f);
	// glBegin(GL_QUADS);
	// glVertex2f(playerX, playerY);
	// glVertex2f(playerX + 0.2f, playerY);
	// glVertex2f(playerX + 0.2f, playerY + 0.2f);
	// glVertex2f(playerX, playerY + 0.2f);
	// glEnd();

	// glBegin(GL_LINES);
	// glVertex2f(playerX + 0.1f, playerY + 0.1f);
	// glVertex2f(playerX + 0.1f + (cos(pAngle) * 0.5), playerY + 0.1f + (sin(pAngle) * 0.5));
	// glEnd();

	// glBegin(GL_LINES);
	// glVertex2f(playerX + 0.1f, playerY + 0.1f);
	// glVertex2f(playerX + 0.1f + (cos(pAngle - M_PI/6) * 100), playerY + 0.1f + (sin(pAngle - M_PI/6) * 100));
	// glEnd();

	// glBegin(GL_LINES);
	// glVertex2f(playerX + 0.1f, playerY + 0.1f);
	// glVertex2f(playerX + 0.1f + (cos(pAngle + M_PI/6) * 100), playerY + 0.1f + (sin(pAngle + M_PI/6) * 100));
	// glEnd();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}

void raycast::renderGame(const int &x1, const int &y1, const int &width, const int &height)
{
	int				rayCnt = 180 * 2;
	int				textureResolution = 64;
	const double	resolution = width / rayCnt;
	const double	maxWallHeight = height;

	if (!window || !map) 
	{
		std::cerr << "Window or map not initialized." << std::endl;
		return;
	}
	
	glViewport(x1, y1, width, height);
	// glClear(GL_COLOR_BUFFER_BIT);
	
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
		double textureStartHorizon = ((mX - (int)mX) * textureResolution);
		double textureStartVertical = ((mY - (int)mY) * textureResolution);
		double wallHeiht = maxWallHeight / abs(cos(pAngle - rAngle) * dist * 0.55);
		double	posY = (maxWallHeight - wallHeiht)/2;
		double	texturePixelHeight = wallHeiht / textureResolution;
		
		if (wallHeiht == maxWallHeight) 
		{
			posY -= 100 / dist;
			texturePixelHeight += 50 / dist;
		}
		
		if(wallHeiht > maxWallHeight)
			wallHeiht = maxWallHeight;

		for (double y = 0; y < textureResolution; ++y) 
		{
			if ((*map)[mY][mX] == '1')
				determineTextureColor(allTextures["textures/Brick_Wall.raw"], dir, y, textureStartHorizon, textureStartVertical);

			glBegin(GL_QUADS);
			glVertex2f(posX,		posY + (y * texturePixelHeight));
			glVertex2f(posX + 1,	posY + (y * texturePixelHeight));
			glVertex2f(posX + 1,	posY + ((y + 1) * texturePixelHeight));
			glVertex2f(posX,		posY + ((y + 1) * texturePixelHeight));
			glEnd();
		}
		// std::cout << "playerY " << playerY << " " <<  height / 2 + wallHeiht --/ 2<<"\n";
		// std::cout << dist << " " << height/2 + wallHeiht / 2 << "\n";
		glEnd();
	}

	// std::cout << width / 2 << " " << height << "\n";
	// std::cout << posX + 1 << " " << posY + ((y + 1) * texturePixelHeight) << "\n";
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void raycast::drawTexture(int startX, int startY) {
	unsigned char ***txtr = allTextures["textures/Floor.raw"];
	std::cout << "startY " << startY <<"\n";
	glBegin(GL_QUADS);
	for (int y = 63; y >startY; y -= 1) {
		for (int x = 0; x < 64; x += 1) {
			glColor4ub(txtr[y][x][0], txtr[y][x][1], txtr[y][x][2], txtr[y][x][3]);
			glVertex2i(x, y);
			glVertex2i(x + 1, y);
			glVertex2i(x + 1, y + 1);
			glVertex2i(x, y + 1);
		}
	}
	glEnd();
}

void raycast::drawBackground(int rayCnt)
{
	// glViewport(0, 0, screenBuffWidth, screenBuffHeight / 6);

	// glMatrixMode(GL_PROJECTION);
	// glPushMatrix();
	// glLoadIdentity();
	// glOrtho(0, 64, 0,64, -1, 1);

	// glMatrixMode(GL_MODELVIEW);
	// glPushMatrix();
	// glLoadIdentity();
	// drawTexture(0, (playerY - (int)playerY) * 64);
	// height / 2 + wallHeiht / 2
	glColor3ub(BLUE_SKY);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(rayCnt, 0);
	glVertex2f(rayCnt, screenBuffHeight / 2);
	glVertex2f(0, screenBuffHeight / 2);
	glEnd();
	////grass/////////
	glColor3ub(GREEN_GRASS);
	glBegin(GL_QUADS);
	glVertex2f(0, screenBuffHeight / 2);
	glVertex2f(rayCnt, screenBuffHeight / 2);
	glVertex2f(rayCnt, screenBuffHeight);
	glVertex2f(0,screenBuffHeight);
	glEnd();
	// glMatrixMode(GL_PROJECTION);
	// glPopMatrix();

	// glMatrixMode(GL_MODELVIEW);
	// glPopMatrix();
}

void raycast::determineTextureColor(unsigned char ***txtr, char dir, int level, int horizon, int verticl)
{
	if(dir == 'h') {
		// glColor4ub(txtr[level][horizon][0], txtr[level][horizon][1], txtr[level][horizon][2], 200);
		glColor4ub(txtr[level][horizon][0], txtr[level][horizon][1], txtr[level][horizon][2], txtr[level][horizon][3]);
	} 
	else {
		int r = txtr[level][verticl][0];
		int g = txtr[level][verticl][1];
		int b = txtr[level][verticl][2];
		int a = txtr[level][verticl][3];

		if(r > 20)	r -= 20;
		else		r = 3;
		if(g > 20)	g -= 20;
		else		g = 3;
		if(b > 20)	b -= 20;
		else 		b = 3;
		
		glColor4ub(r, g, b, a);
		// glColor4ub(r, g, b, 125);
	}
}