#include "BSDF.h"
#include <assert.h>

BSDF::BSDF(const Ray& ray, const HitRecord& hr) 
	: OrthonormalBasis(hr.normal, OB_fromW), nbxdfs(0), gnormal(hr.normal)
{
	const Matrix4x4 m = Transform::makeBasisChange(
		this->localCoordinates.u(),
		this->localCoordinates.v(),
		this->localCoordinates.w()
	);
	this->worldToObj = new Transform(m);
}

void BSDF::add(BxDF* bxdf) {
	assert(this->nbxdfs < GTRACE_MAX_BxDF_COUNT);
	this->bxdfs[this->nbxdfs++] = bxdf;
}
