#pragma once

#include "BxDF.h"
#include "../math/Utils.h"

namespace gtrace {

class LambertianReflection : public BxDF {
public:
	LambertianReflection() : BxDF(BxDFType::Reflection | BxDFType::Specular) {}

	virtual Color3f f(const Color3f& r, const Vector3f& wo, const Vector3f& wi) const {
		return r * PI_1;
	}
};
}
