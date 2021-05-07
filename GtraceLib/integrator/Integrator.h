#pragma once

#include "../scene/World.h"
#include "../Random.h"

class Integrator {
public:
	virtual ~Integrator() {}
	virtual void preprocess(const World&, RandomGenerator& rng) {}
	virtual Vector3f Li(const World& w, const Ray& ray, RandomGenerator& rng, int depth) const = 0;
};