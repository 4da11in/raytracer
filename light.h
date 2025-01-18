#pragma once
#include <iostream>
#include "vec.h"
class light
{
public:
	light(vec color, double intensity);
	vec color;
	double intensity;
};

