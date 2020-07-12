#pragma once

#include "Ray.h"

class BBox {
public:
	BBox() = default;
	BBox(const Vector3f& bottom, const Vector3f& top)
		: bottom(bottom.data()), top(top.data()) {}

	static BBox bound(const BBox& a, const BBox& b);
	Vector3f min() const { return this->bottom; }
	Vector3f max() const { return this->top; }

	bool intersect(const Ray& ray, float tmin, float tmax) const;
private:
	Vector3fData bottom, top;
};