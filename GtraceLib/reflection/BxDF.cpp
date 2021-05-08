#include "BxDF.h"
#include "math/Utils.h"
#include "Sampling.h"

namespace gtrace {

Vector3f BRDFtoBTDF::f(const Color3f& r, const Vector3f& wo, const Vector3f& wi) const {
    return this->brdf->f(r, wo, otherHemisphere(wi));
}

Vector3f BRDFtoBTDF::sample(const Color3f& r, const Ray& ray, const HitRecord& hr, const Vector3f& wo, Vector3f& wi, const Point2f& p, float& pdf) const {
    return this->brdf->sample(r, ray, hr, wo, wi, p, pdf);
}

Vector3f BRDFtoBTDF::rho(const Ray& ray, const HitRecord& hr, uint32_t nSamples, float* samples) const {
    return this->brdf->rho(ray, hr, nSamples, samples);
}

Vector3f BxDF::sample(const Color3f& r, const Ray& ray, const HitRecord& hr, const Vector3f& wo, Vector3f& wi, const Point2f& p, float& pdf) const {
    // Cosine-sample the hemisphere, flipping the direction if necessary
    wi = cosineSampleHemisphere(p);
    if (wi.z() < 0) wi *= Vector3f(1.f, 1.f, -1.f);
    pdf = sameHemisphere(wo, wi) ? cosineHemispherePdf(fabsf(cosTheta(wi))) : 0.f;
    return this->f(r, wo, wi);
}
}
