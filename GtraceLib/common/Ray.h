#pragma once
#include <limits>
#include "math/Vector3f.h"

namespace gtrace
{
struct Ray
{
    Ray(const Vector3f origin, const Vector3f direction) :
        origin(origin),
        direction(direction),
        invDirection(gtrace::min(Vector3f(1.f) / direction, Vector3f(std::numeric_limits<float>::max())))
    {
    }

    Vector3f pointAt(float time) { return origin + time * direction; }

    Vector3f origin;
    Vector3f direction;
    Vector3f invDirection;
};
}  // namespace gtrace
