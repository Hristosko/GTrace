#pragma once

#include "Ray.h"
#include "../scene/Parser.h"

class Shape : public SceneElement{
public:
	virtual ~Shape() {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const = 0;
};