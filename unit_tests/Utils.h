#pragma once

#include "../math/Vector3f.h"
#include "../math/Matrix4x4.h"
#define EPS 0.001

inline bool compare(float a, float b) {
	return fabsf(a - b) < EPS;
}

inline bool compare(const Vector3f & a, const Vector3f & b) {
	return compare(a.x(), b.x()) ||
		compare(a.y(), b.y()) ||
		compare(a.z(), b.z());
}

inline bool compare(const Matrix4x4& a, const Matrix4x4& b) {
	for (int i = 0; i < 4; ++i) {
		if (!compare(a[i], b[i])) return false;
	}
	return true;
}
