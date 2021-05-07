#pragma once

#include "Integrator.h"

class DirectLightIntegrator : public Integrator {
public:
	DirectLightIntegrator(int maxDepth) : maxDepth(maxDepth) {}
	virtual Vector3f Li(const World& w, const Ray& ray, RandomGenerator& rng, int depth) const override;
private:
	int maxDepth;
};