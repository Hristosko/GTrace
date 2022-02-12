#pragma once

#include "Integrator.h"

namespace gtrace {

/**
 * Ray tracing using only direct light. No reflection. No transmission.
 */
class DirectLightIntegrator : public Integrator {
public:
    DirectLightIntegrator(const World& world) : Integrator(world) {}

    virtual Color3f Li(
        RayTracingStat& stat,
        const Ray& ray,
        RandomGenerator& rng,
        int depth) const override;

protected:
    Color3f estimateFromLightSource(
        RayTracingStat& stat,
        const Light* light,
        const HitRecord& hr,
        const Vector3f& wo,
        const BSDF& bsdf,
        RandomGenerator& rng) const;

    Color3f estimateAllLightSources(
        RayTracingStat& stat,
        const HitRecord& hr,
        const Vector3f& wo,
        const BSDF& bsdf,
        RandomGenerator& rng) const;
};
}
