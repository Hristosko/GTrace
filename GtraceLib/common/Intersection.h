#pragma once
#include "math/Vector3f.h"

namespace gtrace
{

class Shape;
struct Intersection
{
    Vector3f point;
    Vector3f normal;
    float time = 0;
    const Shape* intersectdWith = nullptr;

    void set(float time, const Shape* intersectdWith)
    {
        this->time = time;
        this->intersectdWith = intersectdWith;
    }
};

}  // namespace gtrace