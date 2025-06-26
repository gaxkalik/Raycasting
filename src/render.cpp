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
			renderGame(0, 0, screenBuffWidth, screenBuffHeight);
			rendTest(OBJ.getX1(), OBJ.getY1(), screenBuffHeight / 4, screenBuffHeight / 4);
			// renderMinimap(OBJ.getX1(), OBJ.getY1(), OBJ.getWidth(), OBJ.getHeight());
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
	double pStep = playerStep;
	// std::cout << "____________________________\n";
	for (double y = -4; y < 4; y += pStep) {
		for (double x = -4; x < 4; x += pStep) {
			int	mapX = viewPortCenterX + x;
			int	mapY = viewPortCenterY - y;
			if (mapX >= 0 && mapY >=0 && mapX < mapWidth &&
					mapY < mapHeight &&
						(*map)[mapY][mapX] == '1') {
				glColor3f(0.3f, 0.3f, 0.3f); // Gray
			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White
			}
			// std::cout << "x " << mapX << " " << mapY << " Vertex 1: (" << x + 4 << ", " << y + 4 << ") ";
			// std::cout << "Vertex 2: (" << x + 4 + pStep << ", " << y + 4 << ") ";
			// std::cout << "Vertex 3: (" << x + 4 + pStep << ", " << y + 4 + pStep << ") ";
			// std::cout << "Vertex 4: (" << x + 4 << ", " << y + 4 + pStep << ")\n";
			// std::cout << "x " << x + 4 << " " << x + 4 + pStep <<"\n";
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
	int				textureResolution = 16;
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
			determineTextureColor(hWall, dir, y, textureStartHorizon, textureStartVertical);

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

void raycast::determineTextureColor(std::vector<std::string> *texture, char dir, int level, int horizon, int verticl) const
{
	if(dir == 'h') 
	{
		if ((*texture)[level][horizon] == '0')
			glColor3ub(BLACK);
		else if ((*texture)[level][horizon] == '1')
			glColor3ub(WHITE);
		else if ((*texture)[level][horizon] == '2')
			glColor3ub(RED);
		else if ((*texture)[level][horizon] == '3')
			glColor3ub(GREEN);
		else if ((*texture)[level][horizon] == '4')
			glColor3ub(BLUE);
		else if ((*texture)[level][horizon] == '5')
			glColor3ub(GRAY);
		else if ((*texture)[level][horizon] == '6')
			glColor4ub(TRANSPARENT);
	} 
	else 
	{
		if ((*texture)[level][verticl] == '0')
			glColor3ub(BLACK);
		else if ((*texture)[level][verticl] == '1')
			glColor3ub(WHITE_SHADED);
		else if ((*texture)[level][verticl] == '2')
			glColor3ub(RED_SHADED);
		else if ((*texture)[level][verticl] == '3')
			glColor3ub(GREEN_SHADED);
		else if ((*texture)[level][verticl] == '4')
			glColor3ub(BLUE_SHADED);
		else if ((*texture)[level][verticl] == '5')
			glColor3ub(GRAY_SHADED);
		else if ((*texture)[level][verticl] == '6')
			glColor4ub(TRANSPARENT);
	}
}
