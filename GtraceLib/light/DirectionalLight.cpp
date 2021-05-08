#include "DirectionalLight.h"

void DirectionalLight::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser::parseVector3fAndStore(map, "direction", this->direction);
	SceneParser::parseVector3fAndStore(map, "color", this->color);

	this->direction = normalize(this->direction);
}

Vector3f DirectionalLight::sample(const HitRecord& hr, const Point2f& u, Vector3f& wi, float& pdf) const {
	pdf = 1.f;
	wi = -this->direction;
	return this->color;
}
