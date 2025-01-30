#include "pointLight.h"

pointLight::pointLight(vec color, double intensity, vec location) : nonAmbientLight(color, intensity), location(location)
{
}

vec pointLight::getDirection(vec point)
{
    return location - point;
}
