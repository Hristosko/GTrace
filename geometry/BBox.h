#pragma once

#include "Ray.h"

class BBox {
public:
	BBox() = default;
	BBox(const Vector3f& bottom, const Vector3f& top)
		: bottom(bottom), top(top) {}

	const Vector3f& min() const { return this->bottom; }
	const Vector3f& max() const { return this->top; }

	static BBox bound(const BBox& a, const BBox& b);
private:
	Vector3f bottom, top;
};