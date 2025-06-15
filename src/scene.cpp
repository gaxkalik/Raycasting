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

void	scene::addObject(const std::string &name, const obj &o) {
	objs.insert_or_assign(name, o);
}

obj::obj(const int &x, const int &y, const int &sizeX, const int &sizeY, const std::string &name) {
		_x1 = x;
		_x2 = x + sizeX;
		_y1 = y;
		_y2 = y + sizeY;
		_name = name;
		_text = "text";
		_width = sizeX;
		_height = sizeY;
		_tileWidth = 0;
		_tileHeight = 0;
		_brush = '0';
}

obj::obj(const int &x, const int &y, const int &sizeX, const int &sizeY, const int &tile,const std::string &name) {
		_x1 = x;
		_x2 = x + sizeX;
		_y1 = y;
		_y2 = y + sizeY;
		_name = name;
		_text = "text";
		_width = sizeX;
		_height = sizeY;
		_tileWidth = _width / tile;
		_tileHeight = _height / tile;
		_brush = '0';
}