#include "material.h"
#include <iostream>

material::material(double kd, double ks, double ka, vec& od, vec& os, double kgls, double reflectivity, double ior) : 
	kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls), reflectivity(reflectivity), ior(ior)
{

}

vec material::getColor(vec normal, directionalLight mainlight, light ambientLight, pointLight ptLight, vec ptLightDir, vec viewpoint, vec reflectionColor, vec refractionColor)
{
	mainlight.direction = normalize(mainlight.direction);
	viewpoint = normalize(viewpoint);
	vec diffuseDirectional = od*kd*mainlight.intensity * max(dot(normalize(mainlight.direction), normal), 0);
	vec diffusePoint = od*kd*ptLight.intensity * max(dot(normalize(ptLightDir), normal), 0);

	vec ambient = ambientLight.color * ka * od;
	vec rDirectional = normal * dot(normal, normalize(mainlight.direction))*2 - normalize(mainlight.direction);
	vec rPoint = normal * dot(normal, normalize(ptLightDir))*2 - normalize(ptLightDir);
	vec specDirectional = mainlight.color * mainlight.intensity * os * ks * pow(max(dot(viewpoint, rDirectional), 0), kgls);
	vec specPoint = ptLight.color * ptLight.intensity * os * ks * pow(max(dot(viewpoint, rPoint), 0), kgls);
	if (ior == 0) {
		refractionColor = refractionColor*0;
	}
	return colorize(diffuseDirectional+diffusePoint+ambient+specDirectional+specPoint+reflectionColor*reflectivity+refractionColor);
}

