#include "DirectLightIntegrator.h"
#include <assert.h>

Vector3f DirectLightIntegrator::Li(const World& w, const Ray& ray, RandomGenerator& rng, int depth) const {
	return Vector3f();
}

Vector3f DirectLightIntegrator::estimateFromLightSource(
	const World& world, const Light* light, const HitRecord& hr, const Vector3f& wo, const BSDF& bsdf, RandomGenerator& rng) {
	assert(light != nullptr);
	Vector3f res(0.f);
	bool canHitLight = light->canBeHit();

	Vector3f wi;
	float pdfLight;
	const Point2f uLight(rng.get(), rng.get());
	const Vector3f li = light->sample(hr, uLight, wi, pdfLight);

	if (pdfLight > 0.f /*&& !li.isBlack()*/
		&& world.intersect(Ray(hr.position, wi))) {
		const Vector3f f = bsdf.f(wo, wi, BxDFType::AllTypes);
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

Vector3f DirectLightIntegrator::estimateAllLightSources(
	const World& world, const HitRecord& hr, const Vector3f& wo, const BSDF& bsdf, RandomGenerator& rng) {
	Vector3f res(0.f);
	const auto& lights = world.getLights();

	for (Light* light : lights) {
		res += estimateFromLightSource(world, light, hr, wo, bsdf, rng);
	}

	return res;
}

