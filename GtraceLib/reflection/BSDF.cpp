#include "BSDF.h"
#include "../Logger.h"

namespace gtrace {

/**
 * Constructor.
 * Computes the local coordinates and the coordinates transformations.
 */
BSDF::BSDF(const Ray& ray, const HitRecord& hr, const Color3f& r)
	: localCoordinates(hr.normal, OB_fromW()), R(r), nbxdfs(0), gnormal(hr.normal)
{
	const Matrix4x4 m = Transform::makeBasisChange(
		this->localCoordinates.u(),
		this->localCoordinates.v(),
		this->localCoordinates.w()
	);
	this->worldToObj = Transform(m);
}

/**
 * Add new BxDF.
 */
void BSDF::add(const BxDF* bxdf) {
	if (this->nbxdfs >= GTRACE_MAX_BxDF_COUNT)
	{
		LOGERROR("Exceeding max BxDFs per BSDF.");
	}
	else this->bxdfs[this->nbxdfs++] = std::move(bxdf);
}

int BSDF::numComponents(BxDFType flags) const {
	int n = 0;
	for (int i = 0; i < this->nbxdfs; ++i) {
		n += static_cast<int>(BSDF::matchesFlags(*this->bxdfs[i], flags));
	}
	return n;
}

/**
 * See BxDF::f.
 * Computes it for all added bxdf that match the flags.
 */
Color3f BSDF::f(const Vector3f& wo, const Vector3f& wi, BxDFType flags) const {
	// The vectors in the local coordinates
	const Vector3f vo = this->worldToObj.transformDirection(wo);
	const Vector3f vi = this->worldToObj.transformDirection(wi);
	Vector3f res(0.f);

	if (dot(wo, this->gnormal) * dot(wi, this->gnormal) < 0.f)
		flags = (flags & ~BxDFType::Transmission); // we have reflection
	else // we have transmission
		flags = (flags & ~BxDFType::Reflection);

	for (int i = 0; i < this->nbxdfs; ++i) {
		if (BSDF::matchesFlags(*this->bxdfs[i], flags))
			res += this->bxdfs[i]->f(this->R, vo, vi);
	}

	return res;
}
}
