#include "Utils.h"

float sinTheta(const Vector3f& vec) {
	const float as = absSinTheta(vec);
	return (vec.x() < 0.f) ? -as : as;
}

float absSinTheta(const Vector3f& vec) {
	return fabsf(sqrtf(sin2Theta(vec)));
}

float sin2Theta(const Vector3f& vec) {
	const Vector3f& temp = vec * vec;
	return temp.x() + temp.y();
}
