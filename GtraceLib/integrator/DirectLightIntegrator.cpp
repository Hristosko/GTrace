#include "DirectLightIntegrator.h"
#include "Logger.h"
#include <assert.h>

namespace gtrace {

Color3f DirectLightIntegrator::Li(SecondaryRaysStat& stat, const World& w, const Ray& ray, RandomGenerator& rng, int depth) const {
	HitRecord hr;
	bool hitsGeometry = w.intersect(ray, hr);
	if (hitsGeometry == false) return Vector3f(0.f);
	if (hr.mat == nullptr)
	{
		LOGWARNING("Ray hits the geometry but there is no material.");
		return Vector3f(0.f);
	}
	BSDF bsdf = hr.mat->getBSDF(ray, hr);
	return estimateAllLightSources(stat, w, hr, ray.direction, bsdf, rng);
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
	SecondaryRaysStat& stat,
	const World& world, const Light* light, const HitRecord& hr,
	const Vector3f& wo, const BSDF& bsdf, RandomGenerator& rng) {

	assert(light != nullptr);
	Vector3f res(0.f);
	bool canHitLight = light->canBeHit();

	Vector3f wi;
	float pdfLight;
	const Point2f uLight(rng.get(), rng.get());
	const Vector3f li = light->sample(hr, uLight, wi, pdfLight);
	stat.addLightSample();

	// make new record for the next interation
	// inherite some data from the old record
	HitRecord rec = HitRecord::make(hr);
	if (pdfLight > 0.f /*&& !li.isBlack()*/
		// We can hit the same object so we have to moove the position a bit
		// perhaps bump mapping will fix this?
		// if not think of better approach
		&& !world.intersect(Ray(hr.position, wi), rec)) {
		const Vector3f f = bsdf.f(wo, wi, BxDFType::All);
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
	SecondaryRaysStat& stat,
	const World& world, const HitRecord& hr, const Vector3f& wo,
	const BSDF& bsdf, RandomGenerator& rng) {

	Vector3f res(0.f);
	const auto& lights = world.getLights();

	for (Light* light : lights) {
		res += estimateFromLightSource(stat, world, light, hr, wo, bsdf, rng);
	}

	return res;
}
}
