#include "raycast.hpp"

#define OBJ (it->second)

void raycast::window_size_callback(GLFWwindow* window, int width, int height) {
	raycast *rc = static_cast<raycast*>(glfwGetWindowUserPointer(window));
	rc->screenWidth = width;
	rc->screenHeight = height;
	glfwGetFramebufferSize(window, &rc->screenBuffWidth, &rc->screenBuffHeight);
}

void	raycast::addObjectToScene(scene *sc, const int &x1, const int &x2, const int &y1, const int &y2, const std::string &name) {
	sc->addObject(name, obj(x1, x2, y1, y2, name));
}

void	raycast::renderScene(scene &sc) {
	std::map<std::string, obj> &objs = sc.getObjs();
	auto	it = objs.begin();

	for (; it != objs.end(); ++it) {
		if (it->first == "minimap")
			renderMinimap(it->second.getSizeX());
		if (it->first == "mapCreate")
			renderMapCreateToolField(OBJ.getX1(), OBJ.getX2(), OBJ.getY1(), OBJ.getY2());
	}
}

void	raycast::renderMinimap(const int &viewPortSize) {
	double	viewPortCenterX = playerX;
	double	viewPortCenterY = playerY;

	if (!window || !map) {
		std::cerr << "Window or map not initialized." << std::endl;
		return;
	}
	glViewport(0, 0, viewPortSize, viewPortSize);
	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 8, 8, 0, -1, 1);

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
			double	mapY = viewPortCenterY + y;
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
	viewPortCenterY = playerY - viewPortCenterY;
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
}