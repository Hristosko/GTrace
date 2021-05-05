#include "OrthogonalCamera.h"

void OrthogonalCamera::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser::parseVector3fAndStore(map, "look_direction", this->direction);
	SceneParser::parseVector3fAndStore(map, "up", this->up);
	SceneParser::parsefloatAndStore(map, "zoom", this->zoom);
	SceneParser::parsefloatAndStore(map, "height", this->height);

	this->uvw = OrthonormalBasis(this->direction, up, OB_fromWV());
}

Ray OrthogonalCamera::castRay(float px, float py) const {
	px *= zoom;
	py *= zoom;

	const Vector3f position = -this->uvw.u() * px + this->uvw.v() * py - this->uvw.w() * this->height;
	const Vector3f direction = this->uvw.w();

	return Ray(position, direction);
}
