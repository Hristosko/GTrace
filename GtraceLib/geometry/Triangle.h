#pragma once

#include "Shape.h"

namespace gtrace
{
/**
 * Represents a triangle. Mostrly usefull for meshes as utility fiunctions.
 * This class enables using the triangle as a standalone object.
 */
class Triangle
{
public:
    Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* objectToWorld);

    bool hit(const Ray& ray, float tmin, float tmax, float* beta, float* gamma, float* tval) const;
    BBox bbox() const;

private:
    Vector3f a, b, c; /** The tree vertices of the triangle */
    const Transform* objectToWorld;
};
}  // namespace gtrace
