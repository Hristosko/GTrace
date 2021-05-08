#include "DirectionalLight.h"

void DirectionalLight::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	parser.parseVector3fAndStore(map, "direction", this->direction);
	parser.parseVector3fAndStore(map, "color", this->color);

	this->direction = normalize(this->direction);
}

Vector3f DirectionalLight::sample(const HitRecord& hr, const Point2f& u, Vector3f& wi, float& pdf) const {
	pdf = 1.f;
	wi = -this->direction;
	return this->color;
}
