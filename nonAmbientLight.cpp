#include "nonAmbientLight.h"

nonAmbientLight::nonAmbientLight(vec color, double intensity, int samplesu, int samplesv) : 
    light(color, intensity), samplesu(samplesu), samplesv(samplesv) {}