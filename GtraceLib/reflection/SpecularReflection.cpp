#include "SpecularReflection.h"
#include "../math/Utils.h"


Vector3f SpecularReflection::sample(
    const Ray& ray, const HitRecord& hr, const Vector3f& wo, Vector3f& wi, float u1, float u2, float& pdf) const{
    pdf = 1.f; // perfect reflection
    wi = wo * Vector3f(-1.f, -1.f, 1.f);
    return (this->fresnel->evaluate(cosTheta(wo)) * this->R) / fabsf(cosTheta(wi));
}
