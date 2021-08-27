#pragma once

#include "math/Vector3f.h"
#include "math/Vector2f.h"
#include "geometry/Ray.h"
#include "HeapAligned.h"

namespace gtrace {

enum class BxDFType : uint16_t {
	Reflection = 1 << 0,
	Transmission = 1 << 1,
	Diffuse = 1 << 2,
	Glossy = 1 << 3,
	Specular = 1 << 4,
	AllTypes = Diffuse | Glossy | Specular,
	AllReflection = AllTypes | Reflection,
	AllTransmission = AllTypes | Transmission,
	All = AllReflection | AllTransmission
};

inline BxDFType operator|(BxDFType a, BxDFType b) {
	return static_cast<BxDFType>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}

inline BxDFType operator^(BxDFType a, BxDFType b) {
	return static_cast<BxDFType>(static_cast<uint16_t>(a) ^ static_cast<uint16_t>(b));
}

inline BxDFType operator&(BxDFType a, BxDFType b) {
	return static_cast<BxDFType>(static_cast<uint16_t>(a) & static_cast<uint16_t>(b));
}

inline BxDFType operator~(BxDFType a) {
	return static_cast<BxDFType>(~static_cast<uint16_t>(a));
}

class BxDF : public HeapAlignedType<Vector3f> {
public:
	friend class BRDFtoBTDF;
	friend class BSDF;
	BxDF(BxDFType t) : type(t) {}
	virtual ~BxDF() {}

	/**
	 * Compute the reflected ligh (the proportion of each component) alongside given direction.
	 * @param r Factor comming from the texture
	 * @param wo The ray that is leaving the surface
	 * @param wi The ray that hit the surface
	 * @return The reflected light
	 */
	virtual Color3f f(const Color3f& r, const Vector3f& wo, const Vector3f& wi) const = 0;
	/**
	 * Compute the direction of the incomming light (wi) given the outgoing direction.
	 * Uses a cosine distr in the hemisphere
	 * @param r Factor comming from the texture
	 * @param ray The ray that is beeing traced
	 * @param hr The intersection with a surface
	 * @param wo The outgoing light direction, local coordinates
	 * @param wi[out] The direction of the incomming light, local coordinates
	 * @param p random numbers
	 * @pram[out] pdf tthe probability of this incomming direction
	 * @return The reflected light
	 */
	virtual Color3f sample(
		const Color3f& r,
		const Ray& ray, const HitRecord& hr,
		const Vector3f& wo, Vector3f& wi, const Point2f& p, float& pdf) const;

	virtual Vector3f rho(const Ray& ray, const HitRecord& hr, uint32_t nSamples, float* samples = nullptr) const { return Vector3f(0.f); }
protected:
	const BxDFType type;
};

class BRDFtoBTDF : public BxDF {
public:
	BRDFtoBTDF(BxDF *brdf) : BxDF(brdf->type ^ (BxDFType::Transmission | BxDFType::Reflection)), brdf(brdf) {}

	virtual Vector3f f(const Color3f& r, const Vector3f& wo, const Vector3f& wi) const override;
	virtual Vector3f sample(
		const Color3f& r,
		const Ray& ray, const HitRecord& hr,
		const Vector3f& wo, Vector3f& wi, const Point2f& p, float& pdf) const override;

	virtual Vector3f rho(const Ray& ray, const HitRecord& hr, uint32_t nSamples, float* samples = nullptr) const;

private:
	BxDF* brdf;
};
}
