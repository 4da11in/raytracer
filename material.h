#pragma once
#include "light.h"
#include "directionalLight.h"
#include "pointLight.h"

#include <vector>
#include <memory>

class material
{
public:
	material(double kd, double ks, double ka, vec& od, vec& os, double kgls, double reflectivity, double ior=0);
	double kd;
	double ks;
	double ka;
	vec od;
	vec os;
	double kgls;
	double reflectivity;
	double ior;
};

