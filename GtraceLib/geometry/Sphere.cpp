#include "Sphere.h"
#include "BBox.h"
#include "math/Transform.h"

namespace gtrace
{
Sphere::Sphere(const ParsedParams& params) : Shape(params), radius(params.getFloat("radius")) {}

bool Sphere::hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const
{
    if (interection->intersectdWith == this)
        return false;

    const Vector3f origin = objectToWorld->invTransform(ray.origin);
    const Vector3f direction = objectToWorld->invTransformDirection(ray.direction);
    const float a = dot(direction, direction);
    const float b = 2.f * dot(direction, origin);
    const float c = dot(origin, origin) - radius * radius;

    float discr = b * b - 4.f * a * c;
    if (discr > 0.f)
    {
        discr = sqrtf(discr);
        float t = (-b - discr) / (2.f * a);
        if (t < tmin)
        {
            t = (-b + discr) / (2.f * a);
        }
        if (t < tmin || t > tmax)
            return false;

        // we have a hit
        const Vector3f normal = origin + t * direction;
        interection->normal = this->objectToWorld->transformDirection(normal);
        interection->set(t, this);
        return true;
    }
    return false;
}

BBox Sphere::bound() const
{
    const Vector3f rad(this->radius);
    const Vector3f center = this->objectToWorld->transform(Vector3f(0.f));
    return BBox(center - rad, center + rad);
}
}  // namespace gtrace
