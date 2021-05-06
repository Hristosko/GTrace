#pragma once

#include "BxDF.h"
#include "../math/OrthonormalBasis.h"
#include "../geometry/Ray.h"

#define GTRACE_MAX_BxDF_COUNT 8

class BSDF {
public:
	BSDF(const Ray& ray, const HitRecord& hr);

private:
	OrthonormalBasis localCoordinates;
	BxDF* bxdfs;
};