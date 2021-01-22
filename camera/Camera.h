#pragma once

#include "../scene/Parser.h"
#include "../geometry/Ray.h"
#include "../math/OrthonormalBasis.h"
#include "../math/Transform.h"

class Camera : public SceneElement, public ReferenceCounted {
public:
	virtual Ray castRay(float px, float py) const = 0;
};
