#include "Triangle.h"

void Triangle::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& parser = getParser();
	parser.parseVector3fAndStore(map, "a", this->a);
	parser.parseVector3fAndStore(map, "b", this->b);
	parser.parseVector3fAndStore(map, "c", this->c);
	parser.parseTextureAndStore(map, "text", this->text);
}

bool Triangle::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
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
	if (gamma <= 0.f || beta + gamma >= 1.f) return false;

	tval = -dot(temp, AC) / denom;
	if (tval >= tmin && tval <= tmax) {
		rec.t = tval;
		rec.text = this->text;
		rec.normal = normalize(cross(b - a, c - a));
		return true;
	}
	return false;
}

// With ::new the sse types are not alligned propely
void* Triangle::operator new(size_t s) {
	return _mm_malloc(s, 16);
}

void Triangle::operator delete(void* p) {
	_mm_free(p);
}
