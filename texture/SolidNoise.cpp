#include "SolidNoise.h"

#include <math.h>

#include "../Random.h"

SolidNoise::SolidNoise() {
	for (int8_t i = 0; i < 16; ++i) {
		this->phi[i] = i;
	}

	RandomGenerator rng;
	for (int8_t i = 14; i >= 0; --i) {
		int8_t target = int8_t(rng.get() * i);
		std::swap(this->phi[i + 1], this->phi[target]);
	}
}

// cubic wightning function
// assume that |t| < 1
float SolidNoise::omega(float t) const {
	t = fabsf(t);
	const float t2 = t * t;
	const float t4 = t2 * t2;
	const float t6 = t4 * t2;
	return -6.f * t6 + 15.f * t4 - 10.f * t * t2 + 1;
}
