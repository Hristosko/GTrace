#pragma once

#include "../scene/Parser.h"
#include "../HeapAligned.h"

class Light : public SceneElement, public HeapAligned<16> {
public:
	virtual void radiance(Vector3f& direction, Vector3f& color) const = 0;
};