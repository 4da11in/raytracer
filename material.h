#pragma once
#include "light.h"
#include "directionalLight.h"
#include "utilities.h"

class material
{
public:
	material(double kd, double ks, double ka, vec& od, vec& os, double kgls, double ref);
	vec getColor(vec normal, directionalLight mainlight, light ambientlight, vec viewpoint, vec refColor);
	double kd;
	double ks;
	double ka;
	vec od;
	vec os;
	double kgls;
	double ref;
};

