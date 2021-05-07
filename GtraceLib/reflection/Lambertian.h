#pragma once

#include "BxDF.h"
#include "../math/Utils.h"

class LambertianReflection : public BxDF {
public:
	LambertianReflection(const Vector3f& r) : BxDF(BxDFType::Reflection | BxDFType::Specular), R(r * PI_1) {}

	virtual Vector3f f(const Vector3f& wo, const Vector3f& wi) const {
		return R;
	}
private:
	const Vector3f R;
};