#pragma once

#include "Integrator.h"

namespace gtrace {

class DirectLightIntegrator : public Integrator {
public:
	virtual Vector3f Li(SecondaryRaysStat& stat, const World& w,
		const Ray& ray, RandomGenerator& rng, int depth) const override;

protected:
	static Vector3f estimateFromLightSource(
		SecondaryRaysStat& stat,
		const World& world, const Light* light, const HitRecord& hr,
		const Vector3f& wo, const BSDF& bsdf,
		RandomGenerator& rng);
	static Vector3f estimateAllLightSources(
		SecondaryRaysStat& stat,
		const World& world, const HitRecord& hr,
		const Vector3f& wo, const BSDF& bsdf,
		RandomGenerator& rng);
};
}
