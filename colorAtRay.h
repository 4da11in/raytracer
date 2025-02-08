#pragma once
#include <vector>
#include <memory>

#include "vec.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "directionalLight.h"
#include "pointLight.h"
#include "boundingVolumeHierarchy.h"

std::vector<vec> colorAtRay(ray r, boundingVolumeHierarchy bv, boundingVolumeHierarchy originalBVH, int bounceCount, vec bgColor, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights, double zbuffer);
vec getColor(std::vector<vec> intersectionInfo, light ambientlight, std::vector<std::shared_ptr<nonAmbientLight>> lights, std::vector<std::shared_ptr<primitive>> objects, int objectIndex, vec viewpoint, vec reflectionColor, vec refractionColor={0,0,0});
vec colorize(vec v);
double max(double a, double b);
float randFloat(float range);