#include "BBox.h"

namespace gtrace
{
BBox BBox::bound(const BBox& a, const BBox& b)
{
    return BBox(gtrace::min(a.bottom, b.bottom), gtrace::max(a.top, b.top));
}

bool BBox::intersect(const Ray& ray, float tmin, float tmax) const
{
    const Vector3f t0 = (this->bottom - ray.origin) * ray.invDirection;
    const Vector3f t1 = (this->top - ray.origin) * ray.invDirection;
    const Vector3f tsmaller = gtrace::min(t0, t1);
    const Vector3f tbigger = gtrace::max(t0, t1);

    tmin = maxElement(tsmaller, tmin);
    tmax = minElement(tbigger, tmax);
    return tmin <= tmax;
}

float BBox::center(int axis) const
{
    return (max() + min())[axis] * 0.5f;
}

}  // namespace gtrace
