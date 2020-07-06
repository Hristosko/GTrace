#pragma once

#include "../scene/Parser.h"

// temp. defined here
struct Vector2f {
	float x, y;
};

class Texture : public SceneElement {
public:
	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const = 0;
};