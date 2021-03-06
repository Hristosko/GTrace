#pragma once

#include "math/Vector2f.h"
#include "math/Vector3f.h"
#include "math/Utils.h"
#include <algorithm> // std::max

namespace gtrace {

/**
 * Sample a disk with uniform distribution.
 * @param u random numbers with uniform distr in [0, 1)^2
 */
Point2f concentricSampleDisk(const Point2f& u);


/**
 * Sample a hemisphere with a cosine distr.
 * @param u random numbers with uniform distr in [0, 1)^2
 */
Vector3f cosineSampleHemisphere(const Point2f& u);

/**
 * Check if 2 vectors are in the same hemisphere
 */
inline bool sameHemisphere(const Vector3f& a, const Vector3f& b) {
    return a.z() * b.z() > 0;
}

inline float cosineHemispherePdf(float cosTheta) { return cosTheta * PI_1; }
}
