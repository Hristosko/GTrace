#include "Sphere.h"

void Sphere::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& parser = getParser();
	parser.parseVector3fAndStore(map, "center", this->center);
	parser.parseVector3fAndStore(map, "color", this->color);
	parser.parsefloatAndStore(map, "radius", this->radius);
}

bool Sphere::hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const {
	const Vector3f temp = ray.origin - this->center;
	const float a = dot(ray.direction, ray.direction);
	const float b = 2.f * dot(ray.direction, temp);
	const float c = dot(temp, temp) - radius * radius;

	float discr = b * b - 4.f * a * c;
	if (discr > 0.f) {
		discr = sqrtf(discr);
		float t = (-b - discr) / (2.f * a);
		if (t < tmin) {
			t = (-b + discr) / (2.f * a);
		}
		if (t < tmin || t > tmax) return false;

		// we have a hit
		rec.t = t;
		rec.normal = normalize(ray.origin + t * ray.direction - this->center);
		rec.color = this->color;
		return true;
	}
	return false;
}

void* Sphere::operator new(size_t s) {
	return _mm_malloc(s, 16);;
}

void Sphere::operator delete(void* p) {
	_mm_free(p);
}