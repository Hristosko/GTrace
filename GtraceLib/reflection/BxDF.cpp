#include "BxDF.h"
#include "../math/Utils.h"

Vector3f BRDFtoBTDF::f(const Vector3f& wo, const Vector3f& wi) const {
    return this->brdf->f(wo, otherHemisphere(wi));
}

Vector3f BRDFtoBTDF::sample(const Ray& ray, const HitRecord& hr, const Vector3f& wo, Vector3f& wi, float u1, float u2, float& pdf) const {
    return this->brdf->sample(ray, hr, wo, wi, u1, u2, pdf);
}

Vector3f BRDFtoBTDF::rho(const Ray& ray, const HitRecord& hr, uint32_t nSamples, float* samples) const {
    return this->brdf->rho(ray, hr, nSamples, samples);
}
