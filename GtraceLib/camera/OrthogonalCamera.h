#pragma once

#include "Camera.h"
namespace gtrace {

class OrthogonalCamera : public Camera {
public:
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

	virtual Ray castRay(float px, float py) const override;
private:
	Vector3f direction; /** The axis the camera is looking along */
	Vector3f up; /** The up axis (Oy) of the camear */
	OrthonormalBasis uvw; /** The OrthonormalBasis of the camera space */
	float zoom; /** Zoom coefficiat */
	float height; /** The position ot the center of the camera (along the direction axis)= heigh * (-direction)*/
};
}
