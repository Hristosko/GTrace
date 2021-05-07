#pragma once

#include "math/Vector2f.h"
#include "math/Vector3f.h"
#include "math/Utils.h"
#include <algorithm> // std::max

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

inline float CosineHemispherePdf(float cosTheta) { return cosTheta * PI_1; }