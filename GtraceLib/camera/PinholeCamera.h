#pragma once

#include "Camera.h"
namespace gtrace {

class PinholeCamera : public Camera {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

	virtual Ray castRay(float px, float py) const override;
private:
	Vector3f eye; /** The position of the eye, all rays start from here */
	Vector3f lookPoint; /** The point the camera is looking at */
	Vector3f up; /** The up coordinate (Oy) of the camera */
	OrthonormalBasis uvw; /** The OrthonormalBasis of the camera space */
	float viewPlaneDistance; /** The distance between the eye and the view plane (the pixels) */
	float zoom; /** zoom the image */
};
}
