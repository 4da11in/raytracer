#include "colorAtRay.h"
#include <map>

std::vector<vec> colorAtRay(ray r, boundingVolumeHierarchy bv, boundingVolumeHierarchy originalBVH, int bounceCount, vec bgColor, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights, double zbuffer) {
	std::vector<vec> output;
	vec outputColor = colorize(bgColor);
	
	if (bv.primChildren.size() == 0) { // still not at bottom of bounding volume hierarchy
		// vec closestE1(INFINITY, INFINITY, INFINITY);
		// double closestDistSquared = INFINITY;
		int count = 0;
		double volumeTMax = -INFINITY;
		std::multimap<double, std::shared_ptr<boundingVolumeHierarchy>> intersected;
		for (std::shared_ptr<boundingVolumeHierarchy> child : bv.boundingChildren) {
			double volumeT = child->intersects(r);
			// std::cout << "T: " << volumeT << ' ';
			bool originInBox = r.origin.x > child->e1.x && r.origin.x < child->e2.x &&
							r.origin.y > child->e1.y && r.origin.y < child->e2.y &&
							r.origin.z > child->e1.z && r.origin.z < child->e2.z;
			if (volumeT > 0 || originInBox) {
				intersected.insert(std::pair<double, std::shared_ptr<boundingVolumeHierarchy>>(volumeT, child));
			}
		}
		vec finalIntersectionPoint(0,0,0);
		for (std::pair<double, std::shared_ptr<boundingVolumeHierarchy>> child : intersected) {
			// std::cout << "INTERSECTED: " << child.first;
			std::vector<vec> colorInfo = colorAtRay(r, *child.second, originalBVH, bounceCount, bgColor, ambientLight, lights, zbuffer);
			vec intersectionPoint = colorInfo[1];
			
			bool pointInBox = intersectionPoint.x > child.second->e1.x && intersectionPoint.x < child.second->e2.x &&
							intersectionPoint.y > child.second->e1.y && intersectionPoint.y < child.second->e2.y &&
							intersectionPoint.z > child.second->e1.z && intersectionPoint.z < child.second->e2.z;
				outputColor = colorInfo[0];
				finalIntersectionPoint = intersectionPoint;
			if (pointInBox) {
				output.push_back(outputColor);
				output.push_back(intersectionPoint);
				return output;
			}				
		}
		
		output.push_back(outputColor);
		output.push_back(finalIntersectionPoint);
		return output;
	} else { // reached leaf of bvh, run intersections
		std::vector<std::shared_ptr<primitive>> objects = bv.primChildren;
		vec finalIntersectionPoint(0,0,0);
		// Reflections and diffuse
		for (int i = 0; i < objects.size(); i++) {
			std::vector<vec> intersectionInfo = objects[i]->getIntersection(r);
			if (intersectionInfo.size() > 0) {
				vec intersectionPoint = intersectionInfo[0];
				vec normal = intersectionInfo[1];

				// reflection ray
				double eta = 0.0001;
				vec reflectionOrigin = intersectionPoint + normal * eta;
				vec reflectionDirection = r.dir - normal * 2 * dot(r.dir, normal);
				ray reflectionRay(reflectionOrigin, reflectionDirection);
				vec reflectionJitter(randFloat(0.01), randFloat(0.01));
				reflectionRay.dir += reflectionJitter;
				reflectionRay.dir = normalize(reflectionRay.dir);
				// refraction ray
				double ior = objects[i]->mat.ior;
				vec refractionPerp = (r.dir)*1/ior;
				vec refractionParallel = -normal*sqrt(1-refractionPerp.length()*refractionPerp.length());
				vec refractionDirection = refractionParallel + refractionPerp;
				ray refractionRay(intersectionPoint, refractionDirection);
				vec refractionJitter(randFloat(0.001), randFloat(0.001));
				refractionRay.dir += refractionJitter;
				refractionRay.dir = normalize(refractionRay.dir);
				
				if (bounceCount < 3) {
					if (intersectionPoint.z >= zbuffer) {
						vec reflectionColor(0,0,0);
						vec refractionColor(0,0,0);
						if (objects[i]->mat.reflectivity > 0) {
							std::vector<vec> reflectionColorInfo = colorAtRay(reflectionRay, originalBVH, originalBVH, bounceCount + 1, bgColor, ambientLight, lights, zbuffer);
							reflectionColor = reflectionColorInfo[0] * 1.0 / 255;
						}
						if (objects[i]->mat.ior != 0) {
							std::vector<vec> refractionColorInfo = colorAtRay(refractionRay, originalBVH, originalBVH, bounceCount + 1, bgColor, ambientLight, lights, zbuffer);
							refractionColor = refractionColorInfo[0] * 1.0 / 255;
						}
						outputColor = getColor(intersectionInfo, ambientLight, lights, objects, i, -r.dir, reflectionColor, refractionColor);
						zbuffer = intersectionPoint.z;
						finalIntersectionPoint = intersectionPoint;
					}
				}
			}
		}
		output.push_back(outputColor);
		output.push_back(finalIntersectionPoint);
		return output;
	}
}
std::vector<vec> rayCast(ray r, boundingVolumeHierarchy bv, int bounceCount, vec bgColor, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights) {

}
vec getColor(std::vector<vec> intersectionInfo, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights, std::vector<std::shared_ptr<primitive>> objects, int objectIndex, vec viewDir, vec reflectionColor, vec refractionColor)
{
	material mat = objects[objectIndex]->mat;
	vec intersectionPoint = intersectionInfo[0];
	vec normal = intersectionInfo[1];
	viewDir = normalize(viewDir);
	vec diffuse(0,0,0);
	vec spec(0,0,0);
	for (int i = 0; i < lights.size(); i++) {
		std::shared_ptr<nonAmbientLight> light = lights[i];
		int samplesu = light->samplesu;
		int samplesv = light->samplesv;
		for (int sampleu = 0; sampleu < samplesu; sampleu++) {
			for (int samplev = 0; samplev < samplesv; samplev++) {
				vec direction = light->getDirection(intersectionPoint, sampleu, samplev);
				vec randomJitter(randFloat(0.1), randFloat(0.1));
				direction += randomJitter;
				direction = normalize(direction);
				// shadows: if point is occluded, exclude this light sample's contribution
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
		}
		int samples = samplesu*samplesv;
		diffuse /= samples;
		spec /= samples;
	}

	vec ambient = ambientLight.color * ambientLight.intensity * mat.ka * mat.od;
	if (mat.ior == 0) {
		refractionColor = refractionColor*0;
	}
	return colorize(diffuse+ambient+spec+reflectionColor*mat.reflectivity+refractionColor);
}

int clamp(int i, int max) {
    if (i > max)
    {
        return max;
    }
    return i;
}
vec colorize(vec v)
{
    return vec(clamp(int(v.x * 254.999), 255), clamp(int(v.y * 254.999), 255), clamp(int(v.z * 254.999), 255));
}
double max(double a, double b)
{
    if (a > b) {
        return a;
    }
    return b;
}
float randFloat(float range) {
    int random_int = rand();
    return ((random_int % 200) - 100)/100.0 * range;
}