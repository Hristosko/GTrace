#pragma once

#include "Fresnel.h"

/**
 * Returns 100% of the reflection for all incoming directions.
 */
class FresnelNoOp : public Fresnel {
public:
	virtual Vector3f evaluate(float /*cosi*/) const {
		return Vector3f(1.f);
	}
};