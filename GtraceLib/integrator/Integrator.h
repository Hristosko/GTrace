#pragma once

#include "renderer/RendererStat.h"
#include "scene/World.h"
#include "Random.h"

namespace gtrace {

class Integrator {
public:
	virtual ~Integrator() {}
	virtual void preprocess(const World&, RandomGenerator& rng) {}
	virtual Vector3f Li(SecondaryRaysStat& stat, const World& w,
		const Ray& ray, RandomGenerator& rng, int depth) const = 0;
};
}
