#include "Triangle.h"
#include "BBox.h"
#include "math/Transform.h"

namespace gtrace
{
Triangle::Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* objectToWorld) :
    a(a),
    b(b),
    c(c),
    objectToWorld(objectToWorld)
{
}

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float* beta, float* gamma, float* tval) const
{
    const Vector3f& origin = ray.origin;
    const Vector3f& dir = ray.direction;

    const Vector3f v0 = objectToWorld->transform(a);
    const Vector3f v1 = objectToWorld->transform(b);
    const Vector3f v2 = objectToWorld->transform(c);

    const Vector3f v0v1 = v1 - v0;
    const Vector3f v0v2 = v2 - v0;

    const Vector3f pvec = cross(dir, v0v2);
    const float det = dot(v0v1, pvec);

    const float invDet = 1.f / det;

    const Vector3f tvec = origin - v0;
    *beta = dot(tvec, pvec) * invDet;
    if (*beta < 0.f || *beta > 1.f)
        return false;

    const Vector3f qvec = cross(tvec, v0v1);
    *gamma = dot(dir, qvec) * invDet;
    if (*gamma < 0.f || *beta + *gamma > 1.f)
        return false;

    *tval = dot(v0v2, qvec) * invDet;
    return *tval > tmin && *tval < tmax;
}

BBox Triangle::bound() const
{
    const Vector3f A = objectToWorld->transform(a);
    const Vector3f B = objectToWorld->transform(b);
    const Vector3f C = objectToWorld->transform(c);
    const Vector3f bottom = min(min(A, B), C);
    const Vector3f top = max(max(A, B), C);
    return BBox(bottom, top);
}

}  // namespace gtrace
