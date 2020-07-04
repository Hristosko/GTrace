#include "Triangle.h"

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const {
	float tval;

	Vector3f AB = a - b;
	Vector3f AC = a - c;
	Vector3f AO = a - ray.origin;

	const float E = AC.y();
	const float I = ray.direction.z();
	const float H = ray.direction.y();
	const float F = AC.z();
	const float EIHF = E * I - H * F;
	
	const float G = ray.direction.x();
	const float D = AC.x();
	const float GFDI = G * F - D * I;

	const float DHEG = D * H - E * G;

	Vector3f temp(EIHF, GFDI, DHEG);
	const float denom = dot(AB, temp);
	const float beta = dot(AO, temp) / denom;

	if (beta <= 0.f || beta >= 1.f) return false;

	const float A = AB.x();
	const float B = AB.y();
	const float C = AB.z();

	const float J = AO.x();
	const float K = AO.y();
	const float L = AO.z();

	const float AKJB = A * K - J * B;
	const float JCAL = J * C - A * L;
	const float BLKC = B * L - K * C;

	temp = Vector3f(BLKC, JCAL, AKJB);
	const float gamma = dot(temp, ray.direction)/denom;
	if (gamma <= 0.f || gamma >= 1.f) return false;

	tval = -dot(temp, AC) / denom;
	if (tval >= tmin && tval <= tmax) {
		rec.t = tval;
		rec.color = this->color;
		rec.normal = normalize(cross(b - a, c - a));
		return true;
	}
	return false;
}