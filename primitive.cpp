#include "primitive.h"
//virtual std::vector<vec> primitive::getIntersection(ray r) = 0;
primitive::primitive(material mat) : mat(mat) {}