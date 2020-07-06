#pragma once

#include "../scene/Parser.h"
#include "../geometry/Ray.h"

class Material : public SceneElement {
public:
	virtual Vector3f shade(const HitRecord& rec, const Ray& ray) const = 0;
};