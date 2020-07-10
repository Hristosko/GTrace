#pragma once

#include <cstdint>
#include "../math/Vector3f.h"

#define SOLID_NOISE_SIZE 16

class SolidNoise {
public:
	SolidNoise();

	float perlinNoise(const Vector3f& p) const;
private:
	float omega(float t) const;
	Vector3f gamma(int i, int j, int k) const;
	float knot(int i, int j, int k, const Vector3f& v) const;
private:
	int8_t phi[SOLID_NOISE_SIZE];
	static const Vector3f grad[SOLID_NOISE_SIZE];
};