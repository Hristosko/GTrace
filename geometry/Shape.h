#pragma once

#include "Ray.h"

class Shape {
public:
	virtual ~Shape() {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const = 0;
};