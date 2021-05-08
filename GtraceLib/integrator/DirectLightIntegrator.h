#pragma once

#include "Integrator.h"

namespace gtrace {

/**
 * Ray tracing using only direct light. No reflection. No transmission.
 */
class DirectLightIntegrator : public Integrator {
public:
	virtual Color3f Li(SecondaryRaysStat& stat, const World& w,
		const Ray& ray, RandomGenerator& rng, int depth) const override;

protected:
	static Color3f estimateFromLightSource(
		SecondaryRaysStat& stat,
		const World& world, const Light* light, const HitRecord& hr,
		const Vector3f& wo, const BSDF& bsdf,
		RandomGenerator& rng);
	static Color3f estimateAllLightSources(
		SecondaryRaysStat& stat,
		const World& world, const HitRecord& hr,
		const Vector3f& wo, const BSDF& bsdf,
		RandomGenerator& rng);
};
}
