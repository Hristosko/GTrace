#pragma once

#include "Shape.h"

namespace gtrace
{
class Sphere : public Shape
{
public:
    Sphere(const ParsedParams& params);

    bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const override;
    BBox bbox() const override;

private:
    float radius; /** The radius of the sphere */
};
}  // namespace gtrace
