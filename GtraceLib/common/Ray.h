#pragma once
#include "math/Vector3f.h"

namespace gtrace
{
struct Ray
{
    Ray(const Vector3f origin, const Vector3f direction)
        : origin(origin), direction(direction), invDirection(Vector3f(1.f) / direction)
    {
    }

    Vector3f pointAt(float time) { return origin + time * direction; }

    Vector3f origin;
    Vector3f direction;
    Vector3f invDirection;
};
}  // namespace gtrace
