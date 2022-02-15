#pragma once
#include <memory>
#include "Bounded.h"
#include "common/HeapAligned.h"
#include "common/Ray.h"
#include "common/Intersection.h"
#include "parser/ParsedParams.h"

namespace gtrace
{
class BBox;
class Shape : public Bounded<BBox>, HeapAlignedType<Vector3f>
{
public:
    Shape() = default;
    explicit Shape(const ParsedParams& params) : objectToWorld(params.getTransform()) {}
    Shape(const std::shared_ptr<Transform>& tr) : objectToWorld(tr) {}
    virtual ~Shape() = default;

    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const = 0;

protected:
    std::shared_ptr<Transform> objectToWorld; /** Transform world coordiates to obj coordinates and vice versa */
};
}  // namespace gtrace
