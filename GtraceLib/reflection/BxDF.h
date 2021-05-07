#pragma once

#include "../math/Vector3f.h"
#include "../geometry/Ray.h"
#include "../HeapAligned.h"

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

class BxDF : public HeapAligned<16> {
public:
	friend class BRDFtoBTDF;
	friend class BSDF;
	BxDF(BxDFType t) : type(t) {}
	virtual ~BxDF() {}

	virtual Vector3f f(const Vector3f& wo, const Vector3f& wi) const = 0;
	/**
	 * Compute the direction of the incomming light (wi) given the outgoing direction.
	 * @param u1 random number
	 * @param u2 random number
	 * @pram[out] pdf tthe probability of this incomming direction
	 */
	virtual Vector3f sample(
		const Ray& ray, const HitRecord& hr, 
		const Vector3f& wo, Vector3f& wi, float u1, float u2, float& pdf) const = 0;

	virtual Vector3f rho(const Ray& ray, const HitRecord& hr, uint32_t nSamples, float* samples = nullptr) const { return Vector3f(0.f); }
protected:
	const BxDFType type;
};

class BRDFtoBTDF : public BxDF {
public:
	BRDFtoBTDF(BxDF *brdf) : BxDF(brdf->type ^ (BxDFType::Transmission | BxDFType::Reflection)), brdf(brdf) {}

	virtual Vector3f f(const Vector3f& wo, const Vector3f& wi) const;
	virtual Vector3f sample(
		const Ray& ray, const HitRecord& hr,
		const Vector3f& wo, Vector3f& wi, float u1, float u2, float& pdf) const;

	virtual Vector3f rho(const Ray& ray, const HitRecord& hr, uint32_t nSamples, float* samples = nullptr) const;

private:
	BxDF* brdf;
};
