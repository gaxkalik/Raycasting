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
					glColor3f(1.0f, 1.0f, 1.0f);//white
				else if ((*newMap)[y][x] == '1')
					glColor3f(0.4f, 0.4f, 0.4f); // Gray
				else if ((*newMap)[y][x] == '2')
					glColor3f(0.5f, 0.5f, 0.5f); // Gray
				else if ((*newMap)[y][x] == '3')
					glColor3f(0.6f, 0.6f, 0.6f); // Gray
				else if ((*newMap)[y][x] == '4')
					glColor3f(0.7f, 0.7f, 0.7f); // Gray
				else if ((*newMap)[y][x] == 'd')
					glColor3f(0.0f, 0.0f, 1.0f);//blue
				else if ((*newMap)[y][x] == 'p')
					glColor3f(1.0f, 0.0f, 0.0f);//red
				else if ((*newMap)[y][x] == 'c')
					glColor3f(1.0f, 1.0f, 0.0f); //yellow
				else if ((*newMap)[y][x] == 'A')
					glColor3f(0.0f, 0.6f, 0.0f); //green
				else if ((*newMap)[y][x] == 'B')
					glColor3f(0.0f, 0.5f, 0.0f); //green
				else if ((*newMap)[y][x] == 'C')
					glColor3f(0.0f, 0.4f, 0.0f); //green													
				else
					glColor3f(0.0f, 1.0f, 0.0f);//green


				glBegin(GL_QUADS);
				glVertex2f(x, y);
				glVertex2f(x + 1.0f, y);
				glVertex2f(x + 1.0f, y + 1.0f);
				glVertex2f(x, y + 1.0f);
				glEnd();
			}
			
		}
	}


	if(keys.saveMap)
		saveMap();
		
	if(keys.loadCustom)
	{
		//std::cout<< "ewfwfwfwfwfwef";
		system("./rc maps/customMap &\n\n");
		exit(1);
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glDisable(GL_SCISSOR_TEST);

	showTextForMapCreationTool();
}

bool raycast::saveMap() const
{
	std::fstream customMap("maps/customMap", std::ios::out | std::ios::trunc);
	
	customMap<< "T: 1 textures/TECH_2F.raw\n";
	customMap<< "T: 2 textures/TECH_3B.raw\n";
	customMap<< "T: 3 textures/TECH_4F.raw\n";
	customMap<< "T: 4 textures/TECH_4E.raw\n";
	customMap<< "T: A textures/CRATE_2C.raw\n";
	customMap<< "T: B textures/CRATE_1E.raw\n";
	customMap<< "T: C textures/CRATE_1D.raw\n";
	customMap<< "T: d textures/DOOR_1A.raw\n";
	customMap<< "T: c textures/Coin\n";

	for(int i = 0; i < 32; ++i )
	{
		for (int j = 0; j < 32; ++j)
		{
			if(customMap.is_open())
			{
				if((*newMap)[i][j] != 'K')
					customMap << (*newMap)[i][j];
				else
					customMap << '0';
			}
			else
				std::cout << "Couldn't open custom map file"<<std::endl;
		}
		//std::cout << "\n";
		if(i != 31)
			customMap << "\n";
	}

	customMap.close();
	return true;
}

void raycast::showTextForMapCreationTool()
{
	glViewport(0, 0, screenBuffWidth, screenBuffHeight);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, rayCnt, screenBuffHeight, 0, -1, 1);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glEnable(GL_BLEND);											//to use glColor4ub(255, 0, 0, 100); transparent textures
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	drawString(175,50,"map editor");
	drawString(10, 200, screenMessege);

	drawString(13, 895, "save map");
	drawString(36, 1095, "load");


	if(brush != 'p' && brush != 'c' && brush != '0' && brush != 'K')
		draw2DTexture(10,300,brush);
	if(brush == 'c')
		draw2DTexture(10,300,'x');

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void raycast::draw2DTexture(int x,int y, char key)
{
	for(int i=0; i<64; i++)
	{
		for(int j = 0; j < 64; j++)
		{
			unsigned char* current = allTextures[key][j][i];
			int r = current[0];
			int g = current[1];
			int b = current[2];
			int a = current[3];
			drawPoint(x+i,y+6.5*j,r,g,b,a);
		}
	}
}