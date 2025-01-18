#pragma once
#include <vector>

#include "vec.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "directionalLight.h"
#include "utilities.h"

vec colorAtRay(ray r, std::vector<sphere> spheres, std::vector<plane> planes, int bounceCount, vec bgColor, directionalLight directionalLight, light ambientLight);