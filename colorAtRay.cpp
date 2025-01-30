#include "colorAtRay.h"

vec colorAtRay(ray r, std::vector<std::shared_ptr<primitive>> objects, int bounceCount, vec bgColor, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights) {
	vec refColor = bgColor;
	vec outputColor = colorize(bgColor);
	double zbuffer = -999999999;
	vec randomJitter(randFloat(0.01), randFloat(0.01));
	r.dir += randomJitter;
	r.dir = normalize(r.dir);
	// Reflections and diffuse
	for (int i = 0; i < objects.size(); i++) {
		std::vector<vec> intersectionInfo = objects[i]->getIntersection(r);
		if (intersectionInfo.size() > 0) {
			vec intersectionPoint = intersectionInfo[0];
			vec normal = intersectionInfo[1];

			// reflection ray
			double eta = 0.0001;
			vec reflectionOrigin = intersectionPoint + normal * eta;
			vec reflectionDirection = r.dir - normal * 2 * r.dir.dot(normal);
			ray reflectionRay(reflectionOrigin, reflectionDirection);
			// refraction ray
			double ior = objects[i]->mat.ior;
			vec refractionPerp = (r.dir)*1/ior;
			vec refractionParallel = -normal*sqrt(1-refractionPerp.length()*refractionPerp.length());
			vec refractionDirection = refractionParallel + refractionPerp;
			ray refractionRay(intersectionPoint, refractionDirection);
			
			if (bounceCount < 3) {
				if (intersectionPoint.z >= zbuffer) {
					vec reflectionColor = colorAtRay(reflectionRay, objects, bounceCount + 1, bgColor, ambientLight, lights) * 1.0 / 255;
					vec refractionColor = colorAtRay(refractionRay, objects, bounceCount + 1, bgColor, ambientLight, lights) * 1.0 / 255;
					outputColor = getColor(intersectionInfo, ambientLight, lights, objects, i, -r.dir, reflectionColor, refractionColor);
					zbuffer = intersectionPoint.z;
				}				
			}
		}
	}
	return outputColor;
}
vec getColor(std::vector<vec> intersectionInfo, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights, std::vector<std::shared_ptr<primitive>> objects, int objectIndex, vec viewDir, vec reflectionColor, vec refractionColor)
{
	material mat = objects[objectIndex]->mat;
	vec intersectionPoint = intersectionInfo[0];
	vec normal = intersectionInfo[1];
	viewDir = normalize(viewDir);
	vec diffuse(0,0,0);
	vec spec(0,0,0);
	for (int i = 0; i < lights.size(); ++i) {
		std::shared_ptr<nonAmbientLight> light = lights[i];
		vec direction = light->getDirection(intersectionPoint);
		direction = normalize(direction);
		// shadows: if point is occluded, exclude this light's contribution
		bool occluded = false;
		ray toLight(intersectionPoint, direction);
		for (int j = 0; j < objects.size(); j++) {
			if (j != objectIndex) {
				if (objects[j]->getIntersection(toLight).size() > 0) {
					occluded = true;
				}
			}			
		}
		if (!occluded) {
			diffuse += mat.od*mat.kd*light->intensity*max(dot(normalize(direction), normal), 0);
			vec r = normal * dot(normal, normalize(direction))*2 - normalize(direction);
			spec += light->color * light->intensity * mat.os * mat.ks * pow(max(dot(viewDir, r), 0), mat.kgls);
		}		
	}

	vec ambient = ambientLight.color * ambientLight.intensity * mat.ka * mat.od;
	if (mat.ior == 0) {
		refractionColor = refractionColor*0;
	}
	return colorize(diffuse+ambient+spec+reflectionColor*mat.reflectivity+refractionColor);
}