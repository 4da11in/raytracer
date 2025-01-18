#include "material.h"
#include <iostream>

material::material(double kd, double ks, double ka, vec& od, vec& os, double kgls, double ref) : kd(kd), ks(ks), ka(ka), od(od), os(os), kgls(kgls), ref(ref)
{

}

vec material::getColor(vec normal, directionalLight mainlight, light ambientLight, vec viewpoint, vec refColor)
{
	mainlight.direction = normalize(mainlight.direction);
	viewpoint = normalize(viewpoint);
	vec diffuse = od*kd*mainlight.intensity * max(dot(normalize(mainlight.direction), normal), 0);
	vec ambient = ambientLight.color * ka * od;
	vec r = normal * dot(normal, normalize(mainlight.direction))*2 - normalize(mainlight.direction);
	vec spec = mainlight.color * os * ks * pow(max(dot(viewpoint, r), 0), kgls);

	return colorize(diffuse+ambient+spec+refColor*ref);
}

