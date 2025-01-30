#include "areaLight.h"

areaLight::areaLight(double x, double y, double z, vec u, vec v, int samplesu, int samplesv, vec color, double intensity) :
    nonAmbientLight(color, intensity, samplesu, samplesv), samplesu(samplesu), samplesv(samplesv), u(u), v(v), location({x,y,z}) {}
vec areaLight::getDirection(vec point, double sampleu, double samplev)
{
    vec lightPoint = location + u*sampleu/samplesu + v*samplev/samplesv;
    vec scaledu = u*sampleu/samplesu;
    vec scaledv = v*samplev/samplesv;

    return lightPoint - point;
}