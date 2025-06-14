#include "scene.hpp"

scene::scene() {
	std::cout << "[ Scene default constructor called ]" << std::endl;
}

scene::~scene() {
	std::cout << "[ Scene destructor called ]" << std::endl;
}

obj	&scene::getObj(const std::string &name) {
	auto	it = objs.find(name);
	
	return it->second;
}

std::map<std::string, obj> &scene::getObjs(void) { return objs; }

void	scene::addObject(const std::string &name, const obj &o) {
	objs.insert_or_assign(name, o);
}

int	obj::getSizeX(void) { return sizeX; }
int	obj::getSizeY(void) { return sizeY; }
int	obj::getX1(void) { return _y2; }
int	obj::getY1(void) { return _y1; }
int	obj::getX2(void) { return _x2; }
int	obj::getY2(void) { return _y2; }

obj::obj(const int &x1, const int &x2, const int &y1, const int &y2, const std::string &name) {
		_x1 = x1;
		_x2 = x2;
		_y1 = y1;
		_y2 = y2;
		_name = name;
		_text = "text";
		sizeX = _x2 - _x1;
		sizeY = _y2 - _y1;
		tileWidth = 0;
		tileHeight = 0;
}

void	obj::setText(const std::string &text) {
	_text = text;
}