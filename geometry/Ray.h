#pragma once

#include "../math/Vector3f.h"

#define GEOM_EPS 0.0001f

class Renderer;
class Material;

struct Ray {
	Renderer* renderer;
	Vector3f origin;
	Vector3f direction;
	Vector3f invDirection;
	int posneg[3];

	Ray(const Vector3f& origin, const Vector3f& direction)
		: origin(origin), direction(direction),
		invDirection(Vector3f(1.f) / direction) {
		posneg[0] = static_cast<int>(direction.x() > 0.f);
		posneg[1] = static_cast<int>(direction.y() > 0.f);
		posneg[2] = static_cast<int>(direction.z() > 0.f);
	}

	Vector3f pointAt(float t) {
		return this->origin + t * this->direction;
	}
};

struct HitRecord {
	float t;
	Vector3f normal;
	Vector3f position;
	Material* mat;
};