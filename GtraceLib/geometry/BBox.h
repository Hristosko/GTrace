#pragma once

#include "common/Ray.h"
#include "common/HeapAligned.h"

namespace gtrace
{
/**
 * Bounding Box (around a more complex geometry or used by the Bounding volume hierarchy)
 */
class BBox : public HeapAlignedType<Vector3f>
{
public:
    BBox() = default;
    BBox(const Vector3f& bottom, const Vector3f& top) : bottom(bottom), top(top) {}

    static BBox bound(const BBox& a, const BBox& b);
    Vector3f min() const { return bottom; }
    Vector3f max() const { return top; }

    bool intersect(const Ray& ray, float tmin, float tmax) const;

private:
    Vector3f bottom, top;
};
}  // namespace gtrace
