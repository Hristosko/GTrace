#pragma once

#include "Ray.h"
#include "../scene/Parser.h"
#include "../texture/Texture.h"

class Shape : public SceneElement {
public:
	virtual ~Shape() {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const = 0;
};