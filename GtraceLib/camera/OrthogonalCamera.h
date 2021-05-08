#pragma once

#include "Camera.h"

class OrthogonalCamera : public Camera {
public:
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

	virtual Ray castRay(float px, float py) const override;
private:
	Vector3f direction;
	Vector3f up;
	OrthonormalBasis uvw;
	float zoom;
	float height;
};
