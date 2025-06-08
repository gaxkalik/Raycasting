#include "raycast.hpp"

void	raycast::renderMinimap(void) const {
	int	minimapSize;

	if (!window || !map) {
		std::cerr << "Window or map not initialized." << std::endl;
		return;
	}
	if (screenHeight <= screenWidth)
		minimapSize = screenHeight / 4;
	else
		minimapSize = screenWidth / 4;
	glViewport(0, 0, minimapSize, minimapSize);
	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, mapWidth, mapHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glColor3f(0, 0, 0);
	glBegin(GL_QUADS);
	glVertex2f(0, 0);
	glVertex2f(0, minimapSize);
	glVertex2f(minimapSize, minimapSize);
	glVertex2f(minimapSize, 0);
	glEnd();
	for (int y = 0; y < mapHeight; ++y) {
		for (int x = 0; x < mapWidth; ++x) {
			if ((*map)[y][x] == '1') {
				glColor3f(0.5f, 0.5f, 0.5f); // Gray for walls
			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White for empty space
			}
			glBegin(GL_QUADS);
			glVertex2f(x + 0.1f, y + 0.1f);
			glVertex2f(x + 0.9f, y + 0.1f);
			glVertex2f(x + 0.9f, y + 0.9f);
			glVertex2f(x + 0.1f, y + 0.9f);
			glEnd();
		}
	}
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(playerX, playerY);
	glVertex2f(playerX + 0.2f, playerY);
	glVertex2f(playerX + 0.2f, playerY + 0.2f);
	glVertex2f(playerX, playerY + 0.2f);
	glEnd();
}