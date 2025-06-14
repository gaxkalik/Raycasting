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
		obj		&getObj(const std::string &);
		std::map<std::string, obj> &getObjs(void);
	private:
		std::map<std::string, obj>	objs;

};

class obj
{
	public:
		obj(const int &, const int &, const int &, const int &, const std::string &);
		void	setText(const std::string &);
		int		getSizeX(void);
		int		getSizeY(void);
		int		getX1(void);
		int		getY1(void);
		int		getX2(void);
		int		getY2(void);
		~obj() {};
	private:
		int			_x1;
		int			_x2;
		int			_y1;
		int			_y2;
		std::string	_name;
		std::string	_text;
		int			tileWidth;
		int			tileHeight;
		int			sizeX;
		int			sizeY;
};



#endif