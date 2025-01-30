#include "areaLight.h"

areaLight::areaLight(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, vec color, double intensity) :
light(color, intensity), plane(x1, y1, z1, x2, y2, z2, x3, y3, z3)
{
    
}