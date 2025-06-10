#include "raycast.hpp"

void raycast::window_size_callback(GLFWwindow* window, int width, int height) {
	raycast *rc = static_cast<raycast*>(glfwGetWindowUserPointer(window));
	rc->screenWidth = width * 2;
	rc->screenHeight = height * 2;
}

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
	glViewport(screenWidth - minimapSize, 0, minimapSize, minimapSize);
	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 8, 8, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	for (double y = -4; y < 4; y += 0.05) {
		for (double x = -4; x < 4; x += 0.05) {
			if (playerX + x >= 0 && playerY + y >=0 && playerX + x < mapWidth &&
					playerY + y < mapHeight &&
						(*map)[playerY + y][playerX + x] == '1') {
				glColor3f(0.5f, 0.5f, 0.5f); // Gray
			} else {
				glColor3f(1.0f, 1.0f, 1.0f); // White
			}
			glBegin(GL_QUADS);
			glVertex2f(x + 4, y + 4);
			glVertex2f(x + 5, y + 4);
			glVertex2f(x + 5, y + 5);
			glVertex2f(x + 4, y + 5);
			glEnd();
		}
	}
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	glVertex2f(4, 4);
	glVertex2f(4 + 0.2f, 4);
	glVertex2f(4 + 0.2f, 4 + 0.2f);
	glVertex2f(4, 4 + 0.2f);
	glEnd();
}