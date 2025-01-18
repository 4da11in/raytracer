#include "ray.h"
#include "vec.h"

ray::ray(vec& origin, vec& dir) : origin(origin), dir(dir)
{

}

vec ray::at(double t) {
	return origin + (dir * t);
}
