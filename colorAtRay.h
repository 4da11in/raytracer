#pragma once
#include <vector>
#include <memory>

#include "vec.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "directionalLight.h"
#include "utilities.h"

vec colorAtRay(ray r, std::vector<std::shared_ptr<primitive>> objects, int bounceCount, vec bgColor, directionalLight directionalLight, light ambientLight);