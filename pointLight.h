#pragma once
#include "nonAmbientLight.h"
class pointLight : public nonAmbientLight
{
	public:
		pointLight(vec color, double intensity, vec location);
		vec location;
		vec getDirection(vec point);
};

