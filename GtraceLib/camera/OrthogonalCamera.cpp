#include "OrthogonalCamera.h"
namespace gtrace {

void OrthogonalCamera::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	parser.parseVector3fAndStore(map, "look_direction", this->direction);
	parser.parseVector3fAndStore(map, "up", this->up);
	parser.parsefloatAndStore(map, "zoom", this->zoom);
	parser.parsefloatAndStore(map, "height", this->height);

	this->uvw = OrthonormalBasis(this->direction, up, OB_fromWV());
}

Ray OrthogonalCamera::castRay(float px, float py) const {
	px *= zoom;
	py *= zoom;

	const Vector3f position = -this->uvw.u() * px + this->uvw.v() * py - this->uvw.w() * this->height;
	const Vector3f direction = this->uvw.w();

	return Ray(position, direction);
}

}
