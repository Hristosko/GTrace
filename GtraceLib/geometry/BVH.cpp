#include "BVH.h"
#include "common/Logger.h"

namespace gtrace
{
bool BVH::Node::hit(const Ray& ray, float tmin, float tmax, float time, Intersection* intersection) const
{
    if (!bounding.intersect(ray, tmin, tmax))
        return false;

    intersection->time = tmax;  // the left hit might upate that
    // we have to call both branches since we can have 2 hits
    bool ishit = left->hit(ray, tmin, tmax, time, intersection);
    bool ishit1 = right->hit(ray, tmin, intersection->time, time, intersection);

    return ishit || ishit1;
}

bool BVH::hit(const Ray& ray, float tmin, float tmax, float time, Intersection* intersection) const
{
    return tree.data()->hit(ray, tmin, tmax, time, intersection);
}

void BVH::build() { tree.build(); }
void BVH::insert(std::unique_ptr<Shape>&& shape) { tree.insert(std::move(shape)); }

}  // namespace gtrace
