#pragma once

#include "Vector3f.h"

#define PI   3.14159265359f
/// 1/PI
#define PI_1 0.31830988618f

inline float degToRad(float deg) {
	return PI * deg / 180.f;
}


/**
 * Compute the cos of the angla between this unit vector and the Oz axis
 * Theta is the angle in spherical coordinates
 */
inline float cosTheta(const Vector3f& vec) {
	return vec.z();
}

/**
 * Compute the sin of the angla between this unit vector and the Oz axis
 */
float sinTheta(const Vector3f& vec);

/**
 * Compute the |sin| of the angla between this unit vector and the Oz axis
 */
float absSinTheta(const Vector3f& vec);


/**
 * Compute the sin^2 of the angla between this unit vector and the Oz axis
 */
float sin2Theta(const Vector3f& vec);


/**
 * Compute the phi component in spherical coordibates.
 */
inline float cosPhi(const Vector3f& vec) {
	return vec.x() / sinTheta(vec);
}

inline float sinPhi(const Vector3f& vec) {
	return vec.y() / sinTheta(vec);
}