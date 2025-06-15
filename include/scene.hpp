#ifndef SCENE_HPP
#define SCENE_HPP

#include "raycast.hpp"

class obj;

class scene
{
	public:
		scene();
		~scene();
		void	addObject(const std::string &, const obj &);

		obj							&getObj(const std::string &name);
		std::map<std::string, obj>	&getObjs(void) { return objs; };
	private:
		std::map<std::string, obj>	objs;

};

class obj
{
	public:
		obj(const int &, const int &, const int &, const int &, const std::string &);
		obj(const int &, const int &, const int &, const int &, const int &,const std::string &);
		int		getX1() const { return _x1; }
		int		getX2() const { return _x2; }
		int		getY1() const { return _y1; }
		int		getY2() const { return _y2; }
		const	std::string& getName() const { return _name; }
		const	std::string& getText() const { return _text; }
		int		getTileWidth() const { return _tileWidth; }
		int		getTileHeight() const { return _tileHeight; }
		int		getWidth() const { return _width; }
		int		getHeight() const { return _height; }
		char	getBrush() const { return _brush; }

		void	setX1(int x1) { _x1 = x1; }
		void	setX2(int x2) { _x2 = x2; }
		void	setY1(int y1) { _y1 = y1; }
		void	setY2(int y2) { _y2 = y2; }
		void	setName(const std::string& name) { _name = name; }
		void	setText(const std::string& text) { _text = text; }
		void	setTileWidth(int tileWidth) { _tileWidth = tileWidth; }
		void	setTileHeight(int tileHeight) { _tileHeight = tileHeight; }
		void	setWidth(int width) { _width = width; }
		void	setHeight(int height) { _height = height; }
		void	setBrush(char brush) { _brush = brush; }
		~obj() {};
	private:
		int			_x1, _x2, _y1, _y2;
		int			_width, _height;
		int			_tileWidth, _tileHeight;
		char		_brush;
		std::string	_name, _text;
};



#endif