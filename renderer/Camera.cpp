#include "Camera.h"

void Camera::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& parser = getParser();
	parser.parseVector3fAndStore(map, "eye", this->eye);
	parser.parseVector3fAndStore(map, "look_point", this->lookPoint);
	parser.parseVector3fAndStore(map, "up", this->up);
	parser.parsefloatAndStore(map, "view_plane_distance", this->viewPlaneDistance);
	parser.parsefloatAndStore(map, "zoom", this->zoom);

	this->w = normalize(eye - lookPoint);
	this->u = normalize(cross(up, w));
	this->v = cross(w, u);
}

Ray Camera::castRay(float px, float py) const {
	Ray ray;
	ray.origin = this->eye;
	ray.direction = normalize(
		zoom*px*u +
		zoom*py*v -
		viewPlaneDistance*w);
	return ray;
}
