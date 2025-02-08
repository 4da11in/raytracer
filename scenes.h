#include "material.h"
#include "sphere.h"
#include "plane.h"

#include <memory>

std::vector<std::shared_ptr<primitive>> scene1();
std::vector<std::shared_ptr<primitive>> scene2();
std::vector<std::shared_ptr<primitive>> sceneBoundingVolumeDemo();