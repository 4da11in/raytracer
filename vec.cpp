#include "vec.h"

vec::vec(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

vec::vec(double u, double v)
{
    this->x = u;
    this->y = v;
    this->z = 0;
}

vec vec::operator+(vec v)
{
    return vec(this->x+v.x, this->y+v.y, this->z+v.z);
}
vec& vec::operator+=(vec v)
{
    this->x += v.x;
    this->y += v.y;
    this->z += v.z;
    return *this;
}
vec vec::operator-()
{
    return vec(this->x* -1, this->y* -1, this->z * -1);
}
vec vec::operator-(vec v)
{
    return vec(this->x - v.x, this->y - v.y, this->z - v.z);
}
vec vec::operator*(double s)
{
    return vec(this->x * s, this->y * s, this->z * s);
}
vec vec::operator*(vec v)
{
    return vec(this->x * v.x, this->y * v.y, this->z * v.z);
}
vec vec::operator/(double s)
{
    return vec(this->x / s, this->y / s, this->z / s);
}

vec& vec::operator/=(double s)
{
    this->x /= s;
    this->y /= s;
    this->z /= s;
    return *this;
}

double& vec::operator[](int i)
{
    if (i == 0)
        return x;
    if (i == 1)
        return y;
    if (i == 2)
        return z;
    throw std::runtime_error("invalid vector index");
}

double vec::length()
{
    return std::sqrt(x*x + y*y + z*z);
}

std::ostream& operator<<(std::ostream& out, vec& vector)
{
    return out << vector.x << " " << vector.y << " " << vector.z << " ";
}

double dot(vec v1, vec v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

vec cross(vec v1, vec v2) {
return vec(v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x);
}

vec normalize(vec v)
{
    return v / v.length();
}