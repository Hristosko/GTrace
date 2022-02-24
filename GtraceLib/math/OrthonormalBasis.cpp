#include "OrthonormalBasis.h"

#define EPSILON 0.001f
#define EPSILON_SQR EPSILON* EPSILON

namespace gtrace
{
// Some might not work properly !!!

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, OB_fromU)
{
    Vector3f n(1.f, 0.f, 0.f);
    Vector3f m(0.f, 1.f, 0.f);

    vec[0] = normalize(a);
    vec[1] = cross(vec[0], n);
    if (vec[1].lengthSqr() < EPSILON_SQR)
    {
        vec[1] = cross(vec[0], m);
    }
    vec[2] = cross(vec[0], vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, OB_fromV)
{
    Vector3f n(1.f, 0.f, 0.f);
    Vector3f m(0.f, 1.f, 0.f);

    vec[1] = normalize(a);
    vec[0] = cross(vec[1], n);
    if (vec[0].lengthSqr() < EPSILON_SQR)
    {
        vec[0] = cross(vec[1], m);
    }
    vec[2] = cross(vec[0], vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, OB_fromW)
{
    Vector3f n(1.f, 0.f, 0.f);
    Vector3f m(0.f, 1.f, 0.f);

    vec[2] = normalize(a);
    vec[0] = cross(vec[2], n);
    if (vec[0].lengthSqr() < EPSILON_SQR)
    {
        vec[0] = cross(vec[2], m);
    }
    vec[1] = cross(vec[2], vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromUV)
{
    vec[0] = normalize(a);
    vec[2] = normalize(cross(a, b));
    vec[1] = cross(vec[0], vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromUW)
{
    vec[0] = normalize(a);
    vec[1] = normalize(cross(a, b));
    vec[2] = cross(vec[0], vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromVW)
{
    vec[1] = normalize(a);
    vec[0] = normalize(cross(a, b));
    vec[2] = cross(vec[0], vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromWV)
{
    vec[2] = normalize(a);
    vec[0] = normalize(cross(a, b));
    vec[1] = cross(vec[0], vec[2]);
}
}  // namespace gtrace
