#include "OrthonormalBasis.h"

#define EPSILON 0.001f
#define EPSILON_SQR EPSILON*EPSILON

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, OB_fromU) {
	Vector3f n(1.f, 0.f, 0.f);
	Vector3f m(0.f, 1.f, 0.f);

	this->vec[0] = normalize(a);
	this->vec[1] = cross(this->vec[0], n);
	if (this->vec[1].lengthSqr() < EPSILON_SQR) {
		this->vec[1] = cross(this->vec[0], m);
	}
	this->vec[2] = cross(this->vec[0], this->vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, OB_fromV) {
	Vector3f n(1.f, 0.f, 0.f);
	Vector3f m(0.f, 1.f, 0.f);

	this->vec[1] = normalize(a);
	this->vec[0] = cross(this->vec[1], n);
	if (this->vec[0].lengthSqr() < EPSILON_SQR) {
		this->vec[0] = cross(this->vec[1], m);
	}
	this->vec[2] = cross(this->vec[0], this->vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, OB_fromW) {
	Vector3f n(1.f, 0.f, 0.f);
	Vector3f m(0.f, 1.f, 0.f);

	this->vec[2] = normalize(a);
	this->vec[0] = cross(this->vec[2], n);
	if (this->vec[0].lengthSqr() < EPSILON_SQR) {
		this->vec[0] = cross(this->vec[2], m);
	}
	this->vec[1] = cross(this->vec[2], this->vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromUV) {
	this->vec[0] = normalize(a);
	this->vec[2] = normalize(cross(a, b));
	this->vec[1] = cross(this->vec[0], this->vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromUW) {
	this->vec[0] = normalize(a);
	this->vec[1] = normalize(cross(a, b));
	this->vec[2] = cross(this->vec[0], this->vec[1]);
}

OrthonormalBasis::OrthonormalBasis(const Vector3f& a, const Vector3f& b, OB_fromVW) {
	this->vec[1] = normalize(a);
	this->vec[0] = normalize(cross(a, b));
	this->vec[2] = cross(this->vec[0], this->vec[1]);
}
