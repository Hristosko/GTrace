#pragma once

#include "Camera.h"
#include "math/OrthonormalBasis.h"
#include "parser/ParsedParams.h"

namespace gtrace
{
class PinholeCamera : public Camera
{
public:
    PinholeCamera(const ParsedParams& params);
	Ray castRay(float px, float py) const override;
private:
	Vector3f eye; /** The position of the eye, all rays start from here */
	Vector3f lookPoint; /** The point the camera is looking at */
	Vector3f up; /** The up coordinate (Oy) of the camera */
	OrthonormalBasis uvw; /** The OrthonormalBasis of the camera space */
	float viewPlaneDistance; /** The distance between the eye and the view plane (the pixels) */
	float zoom; /** zoom the image */
};
}
