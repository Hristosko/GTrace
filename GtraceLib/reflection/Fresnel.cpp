#include "Fresnel.h"

Vector3f Fresnel::circularyPolariezed(float coso, float cost, const Vector3f& etao, const Vector3f& etat) {
	const Vector3f paralel = (etat * coso - etao * cost) / (etat * coso + etao * cost);
	const Vector3f perpendicular = (etao * coso - etat * cost) / (etao * coso + etat * cost);

	return (paralel * paralel + perpendicular * perpendicular) * 0.5f;
}

Vector3f Fresnel::conductorReflectance(float coso, const Vector3f& eta, const Vector3f& k) {
	Vector3f temp = (eta * eta + k * k) * coso * coso;
	const Vector3f paralelSqr =
		(temp - (2.f * eta * coso) + Vector3f(1.f)) /
		(temp + (2.f * eta * coso) + Vector3f(1.f));

	temp = eta * eta + k * k;
	const Vector3f perpendicularSqr =
		(temp - 2.f * eta * coso + Vector3f(coso * coso)) /
		(temp + 2.f * eta * coso + Vector3f(coso * coso));

	return (paralelSqr + perpendicularSqr) * 0.5f;
}

Vector3f Fresnel::approximateEta(const Vector3f& i) {
	const Vector3f intensity = sqrt(clamp(i, 0.f, 0.999f));
	return (Vector3f(1.f) + intensity) / (Vector3f(1.f) - intensity);
}

Vector3f Fresnel::approximateK(const Vector3f& i) {
	const Vector3f intensity = clamp(i, 0.f, 0.999f);
	return 2.f * sqrt(intensity / (Vector3f(1.f) - intensity));
}
