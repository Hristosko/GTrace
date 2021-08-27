#pragma once

#include "BxDF.h"
#include "Fresnel.h"
#include <memory>

namespace gtrace {

class SpecularReflection : public BxDF {
public:
	SpecularReflection(std::unique_ptr<Fresnel>&& fresnel) :
		BxDF(BxDFType::Reflection | BxDFType::Specular), fresnel(std::move(fresnel)) {}

	virtual Color3f sample(
		const Color3f& r,
		const Ray& ray, const HitRecord& hr,
		const Vector3f& wo, Vector3f& wi, const Point2f& p, float& pdf) const override;

private:
	std::unique_ptr<Fresnel> fresnel;
};
}
