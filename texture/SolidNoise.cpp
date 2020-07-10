#include "SolidNoise.h"

#include <math.h>

#include "../Random.h"

const Vector3f SolidNoise::grad[SOLID_NOISE_SIZE] = {
	Vector3f(1,1,0),
	Vector3f(-1,1,0),
	Vector3f(1,-1,0),
	Vector3f(-1,-1,0),

	Vector3f(1,0,1),
	Vector3f(-1,0,1),
	Vector3f(1,0,-1),
	Vector3f(-1,0,-1),

	Vector3f(0,1,1),
	Vector3f(0,-1,1),
	Vector3f(0,1,-1),
	Vector3f(0,-1,-1),

	Vector3f(1,1,0),
	Vector3f(-1,1,0),
	Vector3f(0,-1,1),
	Vector3f(0,-1,-1),
};

SolidNoise::SolidNoise() {
	for (int8_t i = 0; i < SOLID_NOISE_SIZE; ++i) {
		this->phi[i] = i;
	}

	RandomGenerator rng;
	for (int8_t i = SOLID_NOISE_SIZE - 2; i >= 0; --i) {
		int8_t target = int8_t(rng.get() * i);
		std::swap(this->phi[i + 1], this->phi[target]);
	}
}

float SolidNoise::perlinNoise(const Vector3f& p) const {
	const int fi = static_cast<int>(floor(p.x()));
	const int fj = static_cast<int>(floor(p.y()));
	const int fk = static_cast<int>(floor(p.z()));
	const Vector3f u = p - Vector3f((float)fi, (float)fj, (float)fk);
	double sum = 0.0;

	sum += knot(fi, fj, fk, u);
	sum += knot(fi + 1, fj, fk, u - Vector3f(1.f, 0.f, 0.f));
	sum += knot(fi, fj + 1, fk, u - Vector3f(0.f, 1.f, 0.f));
	sum += knot(fi, fj, fk + 1, u - Vector3f(0.f, 0.f, 1.f));
	sum += knot(fi + 1, fj + 1, fk, u - Vector3f(1.f, 1.f, 0.f));
	sum += knot(fi + 1, fj, fk + 1, u - Vector3f(1.f, 0.f, 1.f));
	sum += knot(fi, fj + 1, fk + 1, u - Vector3f(0.f, 1.f, 1.f));
	sum += knot(fi + 1, fj + 1, fk + 1, u - Vector3f(1.f, 1.f, 1.f));

	return static_cast<float>(sum);
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

Vector3f SolidNoise::gamma(int i, int j, int k) const {
	int idx = phi[abs(k) % SOLID_NOISE_SIZE];
	idx = phi[abs(j + idx) % SOLID_NOISE_SIZE];
	idx = phi[abs(i + idx) % SOLID_NOISE_SIZE];
	return grad[idx];
}

float SolidNoise::knot(int i, int j, int k, const Vector3f& v) const {
	return omega(v.x())* omega(v.y())* omega(v.z())*
		dot(gamma(i, j, k), v);
}
