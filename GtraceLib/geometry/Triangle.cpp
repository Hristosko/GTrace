#include "Triangle.h"
#include "BBox.h"

bool Triangle::hit(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* tr,
	const Ray& ray, float tmin, float tmax,
	float& beta, float& gamma, float& tval) {
	const Vector3f origin = tr->invTransform(ray.origin);
	const Vector3f direction = tr->invTransformDirection(ray.direction);

	Vector3f AB = Vector3f(a) - Vector3f(b);
	Vector3f AC = Vector3f(a) - Vector3f(c);
	Vector3f AO = Vector3f(a) - origin;

	const float E = AC.y();
	const float I = direction.z();
	const float H = direction.y();
	const float F = AC.z();
	const float EIHF = E * I - H * F;

	const float G = direction.x();
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
	gamma = dot(temp, direction) / denom;
	if (gamma <= 0.f || beta + gamma >= 1.f) return false;

	tval = -dot(temp, AC) / denom;
	return tval >= tmin && tval <= tmax;
}

BBox Triangle::triangleBBox(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* tr) {
	const Vector3f A = tr->transform(a);
	const Vector3f B = tr->transform(b);
	const Vector3f C = tr->transform(c);
	const Vector3f bottom = min(min(A, B), C);
	const Vector3f top = max(max(A, B), C);
	return BBox(bottom, top);
}

void Triangle::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser::parseVector3fAndStore(map, "a", this->a);
	SceneParser::parseVector3fAndStore(map, "b", this->b);
	SceneParser::parseVector3fAndStore(map, "c", this->c);
	SceneParser::parseMaterialAndStore(map, "mat", this->mat);
	this->parseTransform(map);

}

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	float tval, beta, gamma;

	if (Triangle::hit(a, b, c, this->objectToWorld.get(), ray, tmin, tmax, beta, gamma, tval)) {
		rec.t = tval;
		rec.mat = this->mat;
		rec.normal = this->objectToWorld->invTransformDirection(cross(b - a, c - a));
		return true;
	}
	return false;
}

BBox Triangle::bbox() const {
	return Triangle::triangleBBox(this->a, this->b, this->c, this->objectToWorld.get());
}
