#pragma once
#include "light.h"
#include "plane.h"
class areaLight : public nonAmbientLight
{
	public:
		areaLight(double x, double y, double z, vec u, vec v, int samplesu, int samplesv, vec color, double intensity);
		vec getDirection(vec point, double sampleu, double samplev);
		int samplesu;
		int samplesv;
		vec location;
		vec u;
		vec v;
};

