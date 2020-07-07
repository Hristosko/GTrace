#include "DirectionalLight.h"

void DirectionalLight::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& p = getParser();
	p.parseVector3fAndStore(map, "direction", this->direction);
	p.parseVector3fAndStore(map, "color", this->color);
}

void DirectionalLight::radiance(Vector3f& direction, Vector3f& color) const {
	direction = this->direction;
	color = this->color;
}
