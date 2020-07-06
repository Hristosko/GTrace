#pragma once

#include "../math/Vector3f.h"
#include "../texture/Texture.h"

#define GEOM_EPS 0.0001f

class Renderer;

struct Ray {
	Renderer* renderer;
	Vector3f origin;
	Vector3f direction;

	Vector3f pointAt(float t) {
		return this->origin + t * this->direction;
	}
};

struct HitRecord {
	float t;
	Vector3f normal;
	Vector3f position;
	Texture* text;
};