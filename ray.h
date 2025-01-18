#pragma once
#include "vec.h"

class ray
{
	public:
		ray(vec& origin, vec& dir);
		vec origin;
		vec dir;
		vec at(double t);
};

