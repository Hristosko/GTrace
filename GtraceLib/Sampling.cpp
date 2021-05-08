#include "Sampling.h"

namespace gtrace {

Point2f concentricSampleDisk(const Point2f& u) {
    // Map uniform random numbers to $[-1,1)^2
    Point2f uOffset = 2.f * u - Vector2f(1, 1);

    const float xOffset = uOffset.x();
    const float yOffset = uOffset.y();
    // Handle degeneracy at the origin
    if (xOffset == 0.f && yOffset == 0.f) return Point2f(0.f);

    // Apply concentric mapping to point
    float theta, r;
    if (fabsf(xOffset) > fabsf(yOffset)) {
        r = xOffset;
        theta = PI * 0.5f - PI * 0.25f * (yOffset / xOffset);
    }
    else {
        r = yOffset;
        theta = PI * 0.5f - PI * 0.25f * (xOffset / yOffset);
    }
    return r * Point2f(cosf(theta), sinf(theta));
}


Vector3f cosineSampleHemisphere(const Point2f& u) {
    Point2f d = concentricSampleDisk(u);
    const float x = d.x();
    const float y = d.y();
    float z = sqrtf(std::max(0.f, 1 - x * x - y * y));
    return Vector3f(x, y, z);
}
}
