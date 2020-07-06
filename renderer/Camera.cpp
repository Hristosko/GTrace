#include "Camera.h"

void Camera::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& parser = getParser();
	parser.parseVector3fAndStore(map, "eye", this->eye);
	parser.parseVector3fAndStore(map, "look_point", this->lookPoint);
	parser.parseVector3fAndStore(map, "up", this->up);
	parser.parsefloatAndStore(map, "view_plane_distance", this->viewPlaneDistance);
	parser.parsefloatAndStore(map, "zoom", this->zoom);

	this->uvw = OrthonormalBasis(eye - lookPoint, up, OB_fromWV());
}

Ray Camera::castRay(float px, float py) const {
	Ray ray;
	ray.origin = this->eye;
	ray.direction = normalize(
		zoom*px*uvw.u() +
		zoom*py*uvw.v() -
		viewPlaneDistance*uvw.w());
	return ray;
}
