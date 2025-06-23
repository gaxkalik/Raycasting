#include "raycast.hpp"

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

	double rAngle = pAngle + M_PI / 6;


	int blockX = (int)playerX;
	int blockY = (int)playerY;

	double rPX = playerX - blockX;
	double rPY = playerY - blockY;				//?


	cout <<"_______________________________________________________________\n";
	for (int i = 0; i < 60; ++i, rAngle -= 0.0174533)
	{

		double vertLengthX = 0;
		double vertLengthY = 0;

		// std::cout << rAngle<<" "<< rAngle * 180 / M_PI << "\n";
		if (rAngle < 0)
			rAngle += 2 * M_PI;
		else if (rAngle > 2 * M_PI)
			rAngle = rAngle - 360 * 0.0174533;
		if ((rAngle >= 0 && rAngle < M_PI_2) || (rAngle > 3 * M_PI_2 && rAngle <= 2 * M_PI))			//right
		{
			vertLengthX = abs(1 - rPX);
			vertLengthY = tan(rAngle) * vertLengthX;
		}
		else if ((rAngle == M_PI_2 || rAngle == 3 * M_PI_2)) {
			vertLengthX = 0;
			vertLengthY = 0;
		}
		else																							//left
		{
			vertLengthX = -abs(rPX);
			vertLengthY = (tan(rAngle) * vertLengthX);
		}
		
		
		double mXV = playerX + vertLengthX;
		double mYV = playerY + vertLengthY;
		double offsetY = tan(rAngle);
		double distV = 100;

		while (mXV >= 0 && mXV < mapWidth && mYV >= 0 && mYV < mapHeight) {
			if ((rAngle >= 0 && rAngle <= M_PI_2) || (rAngle > 3 * M_PI_2 && rAngle <= 2 * M_PI))
			{
				if ((*map)[mYV][mXV] == '1') {
					distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
					break;
				}
				mYV += offsetY;
				++mXV;
			}
			else{
				if (!((*map)[(int)mYV][(int)mXV-1] != '1')) {
					distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
					break;
				}
				--mXV;
				mYV -= offsetY;
			}
		}

		double horLengthX = rPX;
		double horLengthY = rPY;
		double offsetX = 0;


		if(rAngle < M_PI)
		{
			horLengthY = 1 - rPY;
			horLengthX = horLengthY/tan(rAngle);
		}
		if (rAngle> M_PI)	
		{
			horLengthY = -rPY;
			horLengthX = horLengthY/tan(rAngle);
		}
		

		double mXH = playerX + horLengthX;
		double mYH = playerY + horLengthY;
		
		double distH = 100;

		while (mXH >= 0 && mXH < mapWidth && mYH >= 0 && mYH < mapHeight) 
		{
			if(rAngle == 0 || rAngle == M_PI || rAngle == M_PI/2 || rAngle == 3*M_PI/2)
				offsetX = 1;
			else
				offsetX = 1/tan(rAngle);

			//if ((rAngle > 0 && rAngle < M_PI/2)||(rAngle > M_PI/2 && rAngle < M_PI))			//up
			if(rAngle<M_PI)
			{
				if ((*map)[(int)mYH][(int)mXH] == '1') 
				{
					//cout << "    aaaaaaa   " << "mx " << mXH << " my " << mYH << "   ofs " <<offsetX<< "\n";
					distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));			
					break;
				}
				++mYH;
				mXH += offsetX;
			}
			else
			{
				if (!((*map)[(int)mYH-1][(int)mXH] != '1')) 
				{
					
					distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY)) ;
					break;
				}
				--mYH;
				mXH -= offsetX;
			}
		}

		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.1); 
		glVertex2f(playerX, playerY);
		if(distH<=distV && distH != 0)
			glVertex2f(mXH, mYH);
		else
			glVertex2f(mXV, mYV);
		glEnd();
	}

	

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_SCISSOR_TEST);
}

void raycast::renderGame(const int &x1, const int &y1, const int &width, const int &height)
{
	const double resolution = width / 60;
	const double maxWallHeight = height;

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
	glOrtho(0, 60, height, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	////////sky///////
	glColor3f(0.1f, 0.708f, 1.0f);
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(60, 0);
	glVertex2f(60, height/2);
	glVertex2f(0, height/2);
	glEnd();
	//////grass/////////
	glColor3f(0.1f, 0.9f, 0.4f);
	glBegin(GL_QUADS);
	glVertex2f(0, screenBuffHeight/2);
	glVertex2f(60, screenBuffHeight/2);
	glVertex2f(60, screenBuffHeight);
	glVertex2f(0,screenBuffHeight);
	glEnd();
	///////fuck you, daltonik em

	double rAngle = pAngle - M_PI / 6;
	double posX = 0;

	asd = 0;
	double	tmpX = ((mX - (int)mX) * 10);
	//cout << "_______________________________________________\n";
	for (int i = 0; i < 60; ++i, rAngle += 0.0174533, ++posX)
	{
		char dir = 'a';
		double dist = getShortestRay(rAngle, dir);
		++asd;
		double wallHeiht = maxWallHeight / abs(cos(pAngle - rAngle) * dist * 0.55);

		if(wallHeiht > maxWallHeight) 
			wallHeiht = maxWallHeight;

		double posY = (maxWallHeight - wallHeiht)/2;

	
		//cout << i << ". " <<"dist\t" << dist << "\twallHeight\t" << wallHeiht << "\tposX\t " << posX << "\tposY\t" << posY <<"\tres\t"<< resolution <<endl;

		//glColor3f(1.0f, 0.0f, 0.0f);
		double	tmpY = wallHeiht / 10;
		// std::cout << "x - " << tmpX * 8 << " y - " << mY << " "<< asd<< std::endl;
		for (double y = 0; y < 10; ++y) 
		{
				glColor3f(1.0f, 1.0f, 1.0f);
				if(dir == 'h')
					glColor3f(0.4f, 0.4f, 0.4f);
				if ((*hWall)[y][tmpX] == '1')
					glColor3f(0.0f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2f(posX,		posY + (y * tmpY));
				glVertex2f(posX + 1,	posY + (y * tmpY));
				glVertex2f(posX + 1,	posY + ((y + 1) * tmpY));
				glVertex2f(posX,		posY + ((y + 1) * tmpY));
				glEnd();
			}
			tmpX += dist/ 10;
			if (tmpX > 7)
				tmpX = 0;
			std::cout << "________________________________\n";
		// for (double y = 0; y < 8; ++y) 
		// {
		// 	for (double x = tmpX; x < 8; ++x) 
		// 	{
		// 		if ((*hWall)[y][x] == '1') {
		// 			glColor3f(1.0f, 0.0f, 0.0f);
		// 		}
		// 		else {
		// 			std::cout << (*hWall)[y][x];
		// 			glColor3f(0.0f, 0.0f, 1.0f);
		// 		}
		// 		glBegin(GL_QUADS);
		// 		glVertex2f(posX + 0.02,	posY+(y * tmpY));
		// 		glVertex2f(posX + 0.98,	posY+(y * tmpY));
		// 		glVertex2f(posX + 0.98,	posY + tmpY +(y * tmpY)- 0.80);
		// 		glVertex2f(posX  + 0.02,		posY + tmpY+(y * tmpY) - 0.80);
		// 		glEnd();
		// 	}
		// 		std::cout << "\n";
		// }
		// glBegin(GL_QUADS);
		// glVertex2f(posX + 0.02,		posY - 0.05);
		// glVertex2f(posX + 0.98,	posY - 0.05);
		// glVertex2f(posX + 0.98,	posY + wallHeiht - 0.90);
		// glVertex2f(posX  + 0.02,		posY + wallHeiht - 0.90);
		// glEnd();

		// std::cout << "as - " << asd << " " << (int)(dist * 100 / 49)<< std::endl;
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
				distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
				break;
			}
			mYV += offsetY;
			++mXV;
		}
		else{
			if (!((*map)[(int)mYV][(int)mXV-1] != '1')) {
				distV = sqrt((mXV-playerX)*(mXV-playerX) + (mYV-playerY)*(mYV-playerY));
				break;
			}
			--mXV;
			mYV -= offsetY;
		}
	}
	_mX = mXV;
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
				distH = sqrt((mXH-playerX)*(mXH-playerX) + (mYH-playerY)*(mYH-playerY));			
				break;
			}
			++mYH;
			mXH += offsetX;
		}
		else if ((rayAngle >= M_PI && rayAngle <= 3 * M_PI_2) || (rayAngle >= 3 * M_PI_2 && rayAngle <= 2 * M_PI))
		{
			if ((*map)[(int)mYH-1][(int)mXH] == '1') 
			{
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
	_mY = mYH;
	return distH;
}

const double raycast::getShortestRay(double rayAngle, char &dir)
{
	double	xV, yV, xH, yH;
	double	distV = getVerticalRay(rayAngle, xV, yV);
	double	distH = getHorizontalRay(rayAngle, xV, yV);
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
