#include "PinholeCamera.h"

void PinholeCamera::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	parser.parseVector3fAndStore(map, "eye", this->eye);
	parser.parseVector3fAndStore(map, "look_point", this->lookPoint);
	parser.parseVector3fAndStore(map, "up", this->up);
	parser.parsefloatAndStore(map, "view_plane_distance", this->viewPlaneDistance);
	parser.parsefloatAndStore(map, "zoom", this->zoom);

	this->uvw = OrthonormalBasis(eye - lookPoint, up, OB_fromWV());
}

Ray PinholeCamera::castRay(float px, float py) const {
	const Vector3f origin = this->eye;
	const Vector3f direction = normalize(
		zoom * px * uvw.u() +
		zoom * py * uvw.v() -
		viewPlaneDistance * uvw.w());
	return Ray(origin, direction);
}
