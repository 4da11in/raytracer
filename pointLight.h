#pragma once
#include "light.h"
class pointLight
{
	public:
		pointLight(vec color, double intensity, vec location);
		vec location;
		vec color;
		double intensity;
};

