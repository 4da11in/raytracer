#include "material.h"
#include <iostream>

material::material(double kd, double ks, double ka, vec& od, vec& os, double kgls, double reflectivity, double ior) : 
	kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls), reflectivity(reflectivity), ior(ior)
{

}