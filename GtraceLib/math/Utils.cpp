#include "Utils.h"

namespace gtrace
{
float sinTheta(const Vector3f& vec)
{
    return fabsf(sqrtf(sin2Theta(vec)));
}

float sin2Theta(const Vector3f& vec)
{
    const Vector3f& temp = vec * vec;
    return temp.x() + temp.y();
}

}  // namespace gtrace
