#include <math.h>
#include "vec3.h"

vec3::vec3() :
    x(0),
    y(0),
    z(0)
{
}

vec3::vec3(float xNew, float yNew, float zNew) :
    x(xNew),
    y(yNew),
    z(zNew)
{
}

bool vec3::operator==(vec3 rhs) {
    return(x == rhs.x && y == rhs.y && z == rhs.z);
}

vec3 vec3::operator+(vec3 rhs) {
    return vec3( x + rhs.x,
                 y + rhs.y,
                 z + rhs.z);
}

vec3 vec3::operator/(float scalar) {
    return vec3(x / scalar,
                      y / scalar,
                      z / scalar);
}

vec3 vec3::operator-(vec3 rhs)
{
    return vec3( x - rhs.x,
                 y - rhs.y,
                 z - rhs.z);
}

vec3 vec3::operator*(vec3 rhs)
{
    return vec3( x * rhs.x,
                 y * rhs.y,
                 z * rhs.z);
}

vec3 vec3::operator/(vec3 rhs)
{
    return vec3( x / rhs.x,
                 y / rhs.y,
                 z / rhs.z);
}

vec3 vec3::operator+(float scalar)
{
    return vec3( x + scalar,
                 y + scalar,
                 z + scalar);
}

vec3 vec3::operator-(float scalar)
{
    return vec3( x - scalar,
                 y - scalar,
                 z - scalar);
}

vec3 vec3::operator*(float scalar)
{
    return vec3( x * scalar,
                 y * scalar,
                 z * scalar);
}

float vec3::dot(vec3 rhs) {
    return (x * rhs.x +
            y * rhs.y +
            z * rhs.z);
}

vec3 vec3::cross(vec3 rhs) {
    return vec3(y * rhs.z - z * rhs.y,
                z * rhs.x - x * rhs.z,
                x * rhs.y - y * rhs.x);
}

float vec3::length() {
    return float(sqrt( x*x + y*y + z*z ));
}

void vec3::normalize()
{
    float fLength = length();
    if (fLength == 0)
        return;

    x /= fLength;
    y /= fLength;
    z /= fLength;
}
