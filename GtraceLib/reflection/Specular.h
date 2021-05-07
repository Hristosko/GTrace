#pragma once

#include "BxDF.h"
#include "Fresnel.h"
#include <memory>

class SpecularReflection : public BxDF {
public:
	SpecularReflection(const Vector3f& r/*, std::unique_ptr<Fresnel>&& fresnel*/) :
		BxDF(BxDFType::Reflection | BxDFType::Specular), /*fresnel(std::move(fresnel)),*/ R(r) {}

	// return no scatering
	virtual Vector3f f(const Vector3f& wo, const Vector3f& wi) const {
		return R;
	}

	virtual Vector3f sample(
		const Ray& ray, const HitRecord& hr,
		const Vector3f& wo, Vector3f& wi, float u1, float u2, float& pdf) const;

private:
	//std::unique_ptr<Fresnel> fresnel;
	Vector3f R; // scale the reflectance color
};