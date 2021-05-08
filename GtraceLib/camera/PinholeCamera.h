#pragma once

#include "Camera.h"
namespace gtrace {

class PinholeCamera : public Camera {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

	virtual Ray castRay(float px, float py) const override;
private:
	Vector3f eye;
	Vector3f lookPoint;
	Vector3f up;
	OrthonormalBasis uvw;
	float viewPlaneDistance;
	float zoom;
};
}
