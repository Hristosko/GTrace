#pragma once

#include "BxDF.h"
#include "Fresnel.h"

class SpecularReflection : public BxDF {
public:
	SpecularReflection(Fresnel* fresnel, const Vector3f& r) :
		BxDF(BxDFType::Reflection | BxDFType::Specular), fresnel(fresnel), R(r) {}

	// return no scatering
	virtual Vector3f f(const Ray& ray, const HitRecord& hr, const Vector3f& wo, const Vector3f& wi) const {
		return Vector3f(0.f);
	}

	virtual Vector3f sample(
		const Ray& ray, const HitRecord& hr,
		const Vector3f& wo, Vector3f& wi, float u1, float u2, float& pdf) const;

private:
	Fresnel* fresnel;
	Vector3f R; // scale the reflectance color
};