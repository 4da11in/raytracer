#include "pointLight.h"

pointLight::pointLight(vec color, double intensity, vec location) : nonAmbientLight(color, intensity, 1, 1), location(location)
{
}

vec pointLight::getDirection(vec point, double sampleu, double samplev)
{
    return location - point;
}
