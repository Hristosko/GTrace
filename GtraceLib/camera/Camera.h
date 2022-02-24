#pragma once

#include "common/Ray.h"

namespace gtrace
{

/**
 * Represents a camera.
 */
class Camera
{
public:
    virtual ~Camera() = default;
	/**
	 * Cast a ray from the camera.
	 * @param px The x coordinate of the pixel (point)
	 * @param py The y coordinate
	 */
	virtual Ray castRay(float px, float py) const = 0;
};

}
