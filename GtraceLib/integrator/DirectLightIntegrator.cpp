#include "DirectLightIntegrator.h"
#include "Logger.h"
#include <assert.h>

namespace gtrace {

Color3f DirectLightIntegrator::Li(RayTracingStat& stat, const Ray& ray, RandomGenerator& rng, int depth) const {
    HitRecord hr;
    bool hitsGeometry = world.intersect(ray, hr);
    if (hitsGeometry == false) return Vector3f(0.f);
    if (hr.mat == nullptr)
    {
        LOGWARNING("Ray hits the geometry but there is no material.");
        return Vector3f(0.f);
    }
    BSDF bsdf = hr.mat->getBSDF(ray, hr);
    return estimateAllLightSources(stat, hr, ray.direction, bsdf, rng);
}

/**
 * Estimate the ligh contribution of a light source.
 * @param[out] stat Static about the used secondary rays.
 * @param world The wold
 * @param ligh The light source
 * @param hr The intersection point with the geometry
 * @param wo The ray that is leaving the suface
 * @param bsdf The bsdf returned by the material
 * @param rng RNG used to create samples
 * @return The contribution of the light source
 */
Color3f DirectLightIntegrator::estimateFromLightSource(
    RayTracingStat& stat,
    const Light* light,
    const HitRecord& hr,
    const Vector3f& wo,
    const BSDF& bsdf,
    RandomGenerator& rng) const {

    assert(light != nullptr);
    Color3f res(0.f);
    bool canHitLight = light->canBeHit();

    Vector3f wi;
    float pdfLight;
    const Point2f uLight(rng.get(), rng.get());
    const Color3f li = light->sample(hr, uLight, wi, pdfLight);
    stat.addLightSample();

    // make new record for the next interation
    // inherite some data from the old record
    HitRecord rec = HitRecord::make(hr);
    if (pdfLight > 0.f /*&& !li.isBlack()*/
        && !world.intersect(Ray(hr.position, wi), rec)) {
        const Color3f f = bsdf.f(wo, wi, BxDFType::All);
        if (canHitLight == false) {
            res += f * fabsf(dot(wi, hr.normal)) * li / pdfLight;
        }
        else {
            assert(false); // not implemented yet
        }
    }

    if (canHitLight) {
        assert(false); // Sample the bsdf
    }

    return res;
}

/**
 * Estimate the ligh contribution of all light sources.
 * @param[out] stat Static about the used secondary rays.
 * @param world The wold
 * @param hr The intersection point with the geometry
 * @param wo The ray that is leaving the suface
 * @param bsdf The bsdf returned by the material
 * @param rng RNG used to create samples
 * @return The contribution of the light sources
 */
Color3f DirectLightIntegrator::estimateAllLightSources(
    RayTracingStat& stat,
    const HitRecord& hr,
    const Vector3f& wo,
    const BSDF& bsdf,
    RandomGenerator& rng) const {

    Vector3f res(0.f);
    const auto& lights = world.getLights();

    for (const auto& light : lights) {
        res += estimateFromLightSource(stat, light.get(), hr, wo, bsdf, rng);
    }

    return res;
}
}
