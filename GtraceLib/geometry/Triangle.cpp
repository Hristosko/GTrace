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
    const Vector3f origin = objectToWorld->invTransform(ray.origin);
    const Vector3f direction = objectToWorld->invTransformDirection(ray.direction);

    Vector3f AB = Vector3f(a) - Vector3f(b);
    Vector3f AC = Vector3f(a) - Vector3f(c);
    Vector3f AO = Vector3f(a) - origin;

    const float E = AC.y();
    const float I = direction.z();
    const float H = direction.y();
    const float F = AC.z();
    const float EIHF = E * I - H * F;

    const float G = direction.x();
    const float D = AC.x();
    const float GFDI = G * F - D * I;

    const float DHEG = D * H - E * G;

    Vector3f temp(EIHF, GFDI, DHEG);
    const float denom = dot(AB, temp);
    *beta = dot(AO, temp) / denom;

    if (*beta <= 0.f || *beta >= 1.f)
        return false;

    const float A = AB.x();
    const float B = AB.y();
    const float C = AB.z();

    const float J = AO.x();
    const float K = AO.y();
    const float L = AO.z();

    const float AKJB = A * K - J * B;
    const float JCAL = J * C - A * L;
    const float BLKC = B * L - K * C;

    temp = Vector3f(BLKC, JCAL, AKJB);
    *gamma = dot(temp, direction) / denom;
    if (*gamma <= 0.f || *beta + *gamma >= 1.f)
        return false;

    *tval = -dot(temp, AC) / denom;
    return *tval >= tmin && *tval <= tmax;
}

BBox Triangle::bbox() const
{
    const Vector3f A = objectToWorld->transform(a);
    const Vector3f B = objectToWorld->transform(b);
    const Vector3f C = objectToWorld->transform(c);
    const Vector3f bottom = min(min(A, B), C);
    const Vector3f top = max(max(A, B), C);
    return BBox(bottom, top);
}

}  // namespace gtrace
