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
			renderGame(0, 0, screenBuffWidth, screenBuffHeight);
			rendTest(OBJ.getX1(), OBJ.getY1(), screenBuffHeight / 4, screenBuffHeight / 4);
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
			glVertex2f(x + 4, y + 4);
			glVertex2f(x + 4 - pStep, y + 4);
			glVertex2f(x + 4 - pStep, y + 4 - pStep);
			glVertex2f(x + 4, y + 4 - pStep);
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


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}

void raycast::renderGame(const int &x1, const int &y1, const int &width, const int &height)
{
	int				rayCnt = 180 * 2;
	int				textureResolution = textures[0].width;
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
				determineTextureColor(textures[0], dir, y, textureStartHorizon, textureStartVertical);

			glBegin(GL_QUADS);
			glVertex2f(posX,		posY + (y * texturePixelHeight));
			glVertex2f(posX + 1,	posY + (y * texturePixelHeight));
			glVertex2f(posX + 1,	posY + ((y + 1) * texturePixelHeight));
			glVertex2f(posX,		posY + ((y + 1) * texturePixelHeight));
			glEnd();
		}
	}
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void raycast::drawBackground(int rayCnt) const
{
	////////sky///////
	glColor3ub(BLUE_SKY);
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

void raycast::determineTextureColor(texture &currentTexture, char dir, int level, int horizon, int verticl)
{
	if(dir == 'h') {
		glColor4ub(txt[level][horizon][0], txt[level][horizon][1], txt[level][horizon][2], txt[level][horizon][3]);
	} 
	else {
		int r = txt[level][verticl][0];
		int g = txt[level][verticl][1];
		int b = txt[level][verticl][2];
		int a = txt[level][verticl][3];

		if(r > 20)	r -= 20;
		else		r = 3;
		if(g > 20)	g -= 20;
		else		g = 3;
		if(b > 20)	b -= 20;
		else 		b = 3;
		
		glColor4ub(r, g, b, a);
	}
}