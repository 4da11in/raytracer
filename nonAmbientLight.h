#pragma once
#include "light.h"
class nonAmbientLight : public light
{
	public:
		nonAmbientLight(vec color, double intensity, int samplesu, int samplesv);
		virtual vec getDirection(vec point, double sampleu, double samplev) = 0;
		int samplesu;
		int samplesv;
};