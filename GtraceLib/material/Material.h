#pragma once

#include "../scene/Parser.h"
#include "../geometry/Ray.h"
#include "../reflection/BSDF.h"

namespace gtrace {

class Material : public SceneElement {
public:
	virtual BSDF getBSDF(const Ray& ray, const HitRecord& hr) const = 0;
};
}
