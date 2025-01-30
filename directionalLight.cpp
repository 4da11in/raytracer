#include "directionalLight.h"

directionalLight::directionalLight(vec color, double intensity, vec direction) : nonAmbientLight(color, intensity, 1, 1), direction(direction)
{

}