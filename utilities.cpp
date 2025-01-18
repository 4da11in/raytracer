#include "utilities.h"

double dot(vec v1, vec v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec normalize(vec v)
{
    return v / v.length();
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
