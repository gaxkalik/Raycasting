#include "raycast.hpp"

void	raycast::renderMapCreateToolField(const int &x1, const int &y1, const int &width, const int &height) {
	glViewport(x1, y1, width, height);
	glEnable(GL_SCISSOR_TEST);
	glScissor(x1, y1, width, height);

	glClearColor(0.0f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 32, 32, 0, -1, 1);
	
	// glMatrixMode(GL_MODELVIEW);
	// glPushMatrix();
	// glLoadIdentity();
	
	for (double y = 0; y < 32; ++y) {
		for (double x = 0; x < 32; ++x) {
			if ((*newMap)[y][x] == 'K') {
				glColor3f(1.0f, 1.0f, 1.0f); // White
				glBegin(GL_QUADS);
				glVertex2f(x + 0.45f, y + 0.2f);
				glVertex2f(x + 0.55f, y + 0.2f);
				glVertex2f(x + 0.55f, y + 0.8f);
				glVertex2f(x + 0.45f, y + 0.8f);
				
				glVertex2f(x + 0.2f, y + 0.45f);
				glVertex2f(x + 0.2f, y + 0.55f);
				glVertex2f(x + 0.8f, y + 0.55f);
				glVertex2f(x + 0.8f, y + 0.45f);
				glEnd();
			} else 
			{
				if ((*newMap)[y][x] == '0')
					glColor3f(1.0f, 1.0f, 1.0f);
				else if ((*newMap)[y][x] >= '1' && (*newMap)[y][x] <= '9')
					glColor3f(0.3f, 0.3f, 0.3f); // Gray
				else if ((*newMap)[y][x] == 'P')
					glColor3f(1.0f, 0.0f, 0.0f);
				else
					return;
				glBegin(GL_QUADS);
				glVertex2f(x, y);
				glVertex2f(x + 1.0f, y);
				glVertex2f(x + 1.0f, y + 1.0f);
				glVertex2f(x, y + 1.0f);
				glEnd();
			}
			
		}
	}
	
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	// glMatrixMode(GL_MODELVIEW);
	// glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}