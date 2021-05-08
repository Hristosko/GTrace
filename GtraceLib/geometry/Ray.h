#pragma once

#include "math/Vector3f.h"
#include <cstdint>

#define GEOM_EPS 0.0001f

namespace gtrace {

class Renderer;
class Material;

struct Ray {
	uint32_t px, py;
	Renderer* renderer;
	Vector3f origin;
	Vector3f direction;
	Vector3f invDirection;

	Ray(const Vector3f& origin, const Vector3f& direction)
		: origin(origin), direction(direction),
		invDirection(Vector3f(1.f) / direction) {}

	Vector3f pointAt(float t) const {
		return this->origin + t * this->direction;
	}
};

struct HitRecord {
	float t;
	Vector3f normal;
	Vector3f position;
	Material* mat = nullptr;
};
}
