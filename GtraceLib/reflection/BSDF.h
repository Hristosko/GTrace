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
private:
	OrthonormalBasis localCoordinates;
	Ref<Transform> worldToObj;
	int nbxdfs;
	BxDF* bxdfs[GTRACE_MAX_BxDF_COUNT];
};