#pragma once

#include <g3x.h>

G3Xvector operator*(float scalar, const G3Xvector &vec)
{
    G3Xvector res;
    res.x = scalar * vec.x;
    res.y = scalar * vec.y;
    res.z = scalar * vec.z;
    return res;
}

G3Xvector operator*(const G3Xvector &vec, float scalar)
{
    G3Xvector res;
    res.x = scalar * vec.x;
    res.y = scalar * vec.y;
    res.z = scalar * vec.z;
    return res;
}

G3Xvector operator/(const G3Xvector &vec, double scalar)
{
    G3Xvector res;
    res.x = vec.x / scalar;
    res.y = vec.y / scalar;
    res.z = vec.z / scalar;
    return res;
}

G3Xvector operator+(const G3Xvector &vec1, const G3Xvector &vec2)
{
    G3Xvector res;
    res.x = vec1.x + vec2.x;
    res.y = vec1.y + vec2.y;
    res.z = vec1.z + vec2.z;
    return res;
}

G3Xvector operator-(const G3Xvector &first, const G3Xvector &second)
{
    G3Xvector res;
    res.x = first.x - second.x;
    res.y = first.y - second.y;
    res.z = first.z - second.z;
    return res;
}

G3Xvector &operator+=(G3Xvector &first, const G3Xvector &second)
{
    first.x += second.x;
    first.y += second.y;
    first.z += second.z;
    return first;
}

G3Xvector operator+=(const G3Xvector &vec, double scalar)
{
    G3Xvector res;
    res.x = vec.x + scalar;
    res.y = vec.y + scalar;
    res.z = vec.z + scalar;
    return res;
}

G3Xvector &operator-=(G3Xvector &first, const G3Xvector &second)
{
    first.x -= second.x;
    first.y -= second.y;
    first.z -= second.z;
    return first;
}

G3Xvector &operator-=(G3Xvector &first, float scalar)
{
    first.x -= scalar;
    first.y -= scalar;
    first.z -= scalar;
    return first;
}