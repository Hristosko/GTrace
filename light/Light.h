#pragma once

#include "../scene/Parser.h"

class Light : public SceneElement {
public:
	virtual void radiance(Vector3f& direction, Vector3f& color) const = 0;
};