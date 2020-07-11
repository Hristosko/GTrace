#include "Triangle.h"

bool Triangle::hit(const Vector3f& a, const Vector3f& b, const Vector3f& c,
	const Ray& ray, float tmin, float tmax,
	float& beta, float& gamma, float& tval) {

	Vector3f AB = Vector3f(a) - Vector3f(b);
	Vector3f AC = Vector3f(a) - Vector3f(c);
	Vector3f AO = Vector3f(a) - ray.origin;

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
	beta = dot(AO, temp) / denom;

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
	gamma = dot(temp, ray.direction) / denom;
	if (gamma <= 0.f || beta + gamma >= 1.f) return false;

	tval = -dot(temp, AC) / denom;
	return tval >= tmin && tval <= tmax;
}

void Triangle::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& parser = getParser();
	parser.parseVector3fAndStore(map, "a", this->a);
	parser.parseVector3fAndStore(map, "b", this->b);
	parser.parseVector3fAndStore(map, "c", this->c);
	parser.parseMaterialAndStore(map, "mat", this->mat);
}

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	float tval, beta, gamma;

	if (Triangle::hit(a, b, c, ray, tmin, tmax, beta, gamma, tval)) {
		rec.t = tval;
		rec.mat = this->mat;
		rec.normal = normalize(cross(b - a, c - a));
		return true;
	}
	return false;
}
