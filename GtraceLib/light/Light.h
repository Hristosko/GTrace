#pragma once

#include "../scene/Parser.h"
#include "../HeapAligned.h"
#include "../geometry/Ray.h"
#include "../math/Vector2f.h"

class Light : public SceneElement, public HeapAligned<16> {
public:
	virtual Vector3f sample(const HitRecord& hr, const Point2f& u, Vector3f& wi, float& pdf) const = 0;

	virtual bool canBeHit() const { return false; }
};