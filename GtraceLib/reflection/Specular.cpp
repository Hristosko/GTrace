#include "Specular.h"
#include "../math/Utils.h"

namespace gtrace {

Vector3f SpecularReflection::sample(
    const Ray& ray, const HitRecord& hr, const Vector3f& wo, Vector3f& wi, const Point2f& p, float& pdf) const{
    pdf = 1.f; // perfect reflection
    wi = wo * Vector3f(-1.f, -1.f, 1.f);
    return (this->fresnel->evaluate(cosTheta(wo)) * this->R) / fabsf(cosTheta(wi));
}
}
