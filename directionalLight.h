#pragma once
#include "light.h"
class directionalLight
{
	public:
		directionalLight(vec color, double intensity, vec direction);
		vec direction;
		vec color;
		double intensity;
};

