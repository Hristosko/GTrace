#pragma once

#include "BxDF.h"
#include "../math/OrthonormalBasis.h"
#include "../math/Transform.h"
#include "../geometry/Ray.h"
#include "../Ref.h"

#define GTRACE_MAX_BxDF_COUNT 8

class BSDF {
public:
	BSDF(const Ray& ray, const HitRecord& hr);

	void add(BxDF* bxdf);

	int numComponents() const { return this->nbxdfs; }

	int numComponents(BxDFType flags) const;

	Vector3f f(const Vector3f& wo, const Vector3f& wi, BxDFType flags) const;
private:
	static bool matchesFlags(const BxDF& bxdf, BxDFType flags) {
		return (bxdf.type & flags) == bxdf.type;
	}
private:
	OrthonormalBasis localCoordinates;
	Vector3f gnormal;
	Ref<Transform> worldToObj;
	int nbxdfs;
	BxDF* bxdfs[GTRACE_MAX_BxDF_COUNT];
};