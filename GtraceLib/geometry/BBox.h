#pragma once

#include "Ray.h"
#include "HeapAligned.h"
namespace gtrace {

/**
 * Bounding Box (around a more complex geometry or used by the Bounding volume hierarchy)
 */
class BBox : public HeapAligned<16> {
public:
	BBox() = default;
	BBox(const Vector3f& bottom, const Vector3f& top)
		: bottom(bottom), top(top) {}

	static BBox bound(const BBox& a, const BBox& b);
	Vector3f min() const { return this->bottom; }
	Vector3f max() const { return this->top; }

	bool intersect(const Ray& ray, float tmin, float tmax) const;
private:
	Vector3f bottom, top;
};
}
