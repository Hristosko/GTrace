#pragma once

#include "../math/Vector3f.h"

#define GEOM_EPS 0.0001f

struct Ray {
	Vector3f origin;
	Vector3f direction;

	Vector3f pointAt(float t) {
		return this->origin + t * this->direction;
	}
};

struct HitRecort {
	float t;
	Vector3f normal;
	Vector3f color;
};