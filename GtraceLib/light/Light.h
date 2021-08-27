#pragma once

#include "scene/Parser.h"
#include "HeapAligned.h"
#include "geometry/Ray.h"
#include "math/Vector2f.h"

namespace gtrace {

/**
 * Represents a light source.
 */
class Light : public SceneElement, public HeapAlignedType<Vector3f> {
public:
	/**
     * Sample the source. Find a ray that can hit the given point.
	 * @param hr The intersection point with the geometry
	 * @param u Uniformly sampled numbers in [0, 1)
	 * @param wi[out] The ray from the light source that hit the surface
	 * @param pdf[out] The probability that this is the ray
	 * @return The color of the light that hit the surface
	 */
	virtual Color3f sample(const HitRecord& hr, const Point2f& u, Vector3f& wi, float& pdf) const = 0;

	/**
	 * Checks if the light souce can br hit by a ray. If this returns true the integrator should
	 * sample the bsdf, not only the light source.
	 */
	virtual bool canBeHit() const { return false; }
};
}
