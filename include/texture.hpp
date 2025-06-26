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
	int					width, height, clCnt, pxLen;
	std::vector<std::string>		tx;
	std::map<std::string, color>	cl;
};



#endif