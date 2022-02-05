#pragma once

#include "BxDF.h"
#include "math/OrthonormalBasis.h"
#include "math/Transform.h"
#include "geometry/Ray.h"

namespace gtrace {

#define GTRACE_MAX_BxDF_COUNT 8

/**
 * Composes multiple BxDFs. Returned by the materials.
 */
class BSDF {
public:
	BSDF(const Ray& ray, const HitRecord& hr, const Color3f& r);

	void add(const BxDF* bxdf);

	int numComponents() const { return this->nbxdfs; }

	int numComponents(BxDFType flags) const;

	Color3f f(const Vector3f& wo, const Vector3f& wi, BxDFType flags) const;
private:
	static bool matchesFlags(const BxDF& bxdf, BxDFType flags) {
		return (bxdf.type & flags) == bxdf.type;
	}
private:
	OrthonormalBasis localCoordinates; /** The local coordibates and the intersection point, Oz = normal */
	Vector3f gnormal; /** The normal to the geometry */
	Color3f R; /** The color computed from the texture */
	Transform worldToObj; /** world to local coordinates Transform */
	int nbxdfs; /** number of bxdfs */
	const BxDF* bxdfs[GTRACE_MAX_BxDF_COUNT]; /** The bxdfs */
};
}
