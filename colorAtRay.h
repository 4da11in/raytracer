#pragma once
#include <vector>
#include <memory>

#include "vec.h"
#include "ray.h"
#include "sphere.h"
#include "planePrimitive.h"
#include "directionalLight.h"
#include "pointLight.h"

vec colorAtRay(ray r, std::vector<std::shared_ptr<primitive>> objects, int bounceCount, vec bgColor, light ambientLight, std::vector<std::shared_ptr<nonAmbientLight>> lights);
vec getColor(std::vector<vec> intersectionInfo, light ambientlight, std::vector<std::shared_ptr<nonAmbientLight>> lights, std::vector<std::shared_ptr<primitive>> objects, int objectIndex, vec viewpoint, vec reflectionColor, vec refractionColor={0,0,0});
vec colorize(vec v);
double max(double a, double b);
float randFloat(float range);