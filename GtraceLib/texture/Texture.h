#pragma once

#include "../scene/Parser.h"
#include "../HeapAligned.h"
#include "../math/Vector2f.h"

#define DEFAULT_TEXTURE_VALUE Vector3f(0.5f)

namespace gtrace {

class Texture : public SceneElement, public HeapAligned<16> {
public:
	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const = 0;
};

inline Vector3f getTextureValue(const Texture* text, const Vector2f& uv, const Vector3f& p) {
	return (text == nullptr) ?
		DEFAULT_TEXTURE_VALUE :
		text->getValue(uv, p);
}
}
