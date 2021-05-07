#pragma once

#include "../scene/Parser.h"
#include "../geometry/Ray.h"
#include "../reflection/BSDF.h"

class Material : public SceneElement {
public:
	virtual Vector3f shade(const HitRecord& rec, const Ray& ray) const = 0;
	virtual BSDF getBSDF(const Ray& ray, const HitRecord& hr) const = 0;
};