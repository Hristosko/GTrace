#pragma once

#include "Vector3f.h"

namespace gtrace
{
#define PI 3.14159265359f
/// 1/PI
#define PI_1 0.31830988618f

inline float degToRad(float deg)
{
    return PI * deg / 180.f;
}

/**
 * Compute the cos of the angla between this unit vector and the Oz axis
 * Theta is the angle in spherical coordinates
 * Theta is in [0, pi]
 * Phi is in [0, 2pi]
 */
inline float cosTheta(const Vector3f& vec)
{
    return vec.z();
}

/**
 * Compute the sin of the angla between this unit vector and the Oz axis. Always > 0
 */
float sinTheta(const Vector3f& vec);

/**
 * Compute the sin^2 of the angla between this unit vector and the Oz axis
 */
float sin2Theta(const Vector3f& vec);

/**
 * Compute the phi component in spherical coordibates.
 */
inline float cosPhi(const Vector3f& vec)
{
    return vec.x() / sinTheta(vec);
}

inline float sinPhi(const Vector3f& vec)
{
    return vec.y() / sinTheta(vec);
}

inline Vector3f otherHemisphere(const Vector3f& vec)
{
    return vec * Vector3f(1.f, 1.f, -1.f);
}
}  // namespace gtrace
