#include "DirectionalLight.h"

void DirectionalLight::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser::parseVector3fAndStore(map, "direction", this->direction);
	SceneParser::parseVector3fAndStore(map, "color", this->color);

	this->direction = normalize(this->direction);
}

void DirectionalLight::radiance(Vector3f& direction, Vector3f& color) const {
	direction = this->direction;
	color = this->color;
}
