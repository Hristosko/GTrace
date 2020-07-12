#pragma once

#include "../math/Vector3f.h"
#define EPS 0.001

inline bool compare(float a, float b) {
	return fabsf(a - b) < EPS;
}

inline bool compare(const Vector3f & a, const Vector3f & b) {
	return compare(a.x(), b.x()) ||
		compare(a.y(), b.y()) ||
		compare(a.z(), b.z());
}
