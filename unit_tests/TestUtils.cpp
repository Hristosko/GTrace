#include "TestUtils.h"
#include "common/Random.h"

namespace gtrace
{
bool compare(const Vector3f& a, const Vector3f& b, bool cmp4)
{
    return compare(a.x(), b.x()) && compare(a.y(), b.y()) && compare(a.z(), b.z()) &&
           (cmp4 == false || compare(a[3], b[3]));
}

bool compare(const Matrix4x4& a, const Matrix4x4& b)
{
    for (int i = 0; i < 4; ++i)
    {
        if (!compare(a[i], b[i], true))
            return false;
    }
    return true;
}

Vector3f randomVector3f()
{
    RandomGenerator rg;
    return Vector3f(rg.get(), rg.get(), rg.get());
}

}  // namespace gtrace
