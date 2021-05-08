#pragma once

#include "renderer/RendererStat.h"
#include "scene/World.h"
#include "Random.h"

namespace gtrace {

/**
 * Implements a ray tracing algorithm.
 */
class Integrator {
public:
	virtual ~Integrator() {}
	virtual void preprocess(const World&, RandomGenerator& rng) {}

	/**
     * Trace a single ray and determine the color of the intersection point
	 * with the geometry.
	 * @param[out] stat Static about the used secondary rays.
	 * @param w The world
	 * @param ray The ray
	 * @param rng RNG used to create samples
	 * @param depth The depth of the current ray, initialy 0
	 * @return The color of the intersection point
	 */
	virtual Color3f Li(SecondaryRaysStat& stat, const World& w,
		const Ray& ray, RandomGenerator& rng, int depth) const = 0;
};
}
