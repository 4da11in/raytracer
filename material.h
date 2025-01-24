#pragma once
#include "light.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "utilities.h"

class material
{
public:
	material(double kd, double ks, double ka, vec& od, vec& os, double kgls, double reflectivity, double ior=0);
	vec getColor(vec normal, directionalLight mainlight, light ambientlight, pointLight ptLight, vec ptLightDir, vec viewpoint, vec reflectionColor, vec refractionColor={0,0,0});
	double kd;
	double ks;
	double ka;
	vec od;
	vec os;
	double kgls;
	double reflectivity;
	double ior;
};

