#pragma once

#include "../scene/Parser.h"
#include "../geometry/Ray.h"
#include "../math/OrthonormalBasis.h"
#include "../math/Transform.h"

namespace gtrace {

/**
 * Represents a camera.
 */
class Camera : public SceneElement {
public:
	/**
	 * Cast a ray from the camera.
	 * @param px The x coordinate of the pixel (point)
	 * @param py The y coordinate
	 */
	virtual Ray castRay(float px, float py) const = 0;
};

}
