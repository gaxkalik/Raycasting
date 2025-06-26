#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <stdlib.h>
#include <map>

struct color
{
	int r, g, b;
};

struct texture
{
	std::vector<std::string>	texture;
	std::map<char, color>		cl;
};



#endif