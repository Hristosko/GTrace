#pragma once

#include "Shape.h"

namespace gtrace
{
/**
 * Represents a triangle. Mostrly usefull for meshes as utility fiunctions.
 * This class enables using the triangle as a standalone object.
 */
struct Triangle
{
    Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* objectToWorld);

    bool hit(const Ray& ray, float tmin, float tmax, float* beta, float* gamma, float* tval) const;
    BBox bound() const;

    /** The tree vertices of the triangle */
    const Vector3f& a;
    const Vector3f& b;
    const Vector3f& c;
    const Transform* objectToWorld;
};
}  // namespace gtrace
