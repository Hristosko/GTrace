#pragma once

#include "math/Vector3f.h"

namespace gtrace {

class Fresnel {
public:
	virtual ~Fresnel() {}

	virtual Vector3f evaluate(float cosi) const = 0;
protected:
	static Vector3f circularyPolariezed(float coso, float cost, const Vector3f& etao, const Vector3f& etat);
	static Vector3f conductorReflectance(float coso, const Vector3f& eta, const Vector3f& k);
	static Vector3f approximateEta(const Vector3f& i);
	static Vector3f approximateK(const Vector3f& i);
};
}
