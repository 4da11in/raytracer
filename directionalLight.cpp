#include "directionalLight.h"

directionalLight::directionalLight(vec color, double intensity, vec direction) : nonAmbientLight(color, intensity), direction(direction)
{

}