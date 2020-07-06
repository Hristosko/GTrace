#pragma once

#include "../scene/Parser.h"
#include "../geometry/Ray.h"

class Camera : public SceneElement {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;

	Ray castRay(float px, float py) const;
private:
	Vector3f eye;
	Vector3f lookPoint;
	Vector3f up;
	Vector3f u, v, w;
	float viewPlaneDistance;
	float zoom;
};