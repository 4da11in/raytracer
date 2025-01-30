#pragma once
#include "light.h"
class nonAmbientLight : public light
{
	public:
		nonAmbientLight(vec color, double intensity);
		virtual vec getDirection(vec point) = 0;
};