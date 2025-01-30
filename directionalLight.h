#pragma once
#include "nonAmbientLight.h"
class directionalLight : public nonAmbientLight
{
	public:
		directionalLight(vec color, double intensity, vec direction);
		vec direction;
		vec getDirection(vec point) {
			return direction; // directional light ignores intersection point
		};
};

