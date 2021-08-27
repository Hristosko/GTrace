#pragma once

#include "scene/Parser.h"
#include "HeapAligned.h"
#include "math/Vector2f.h"

#define DEFAULT_TEXTURE_VALUE Vector3f(0.5f)

namespace gtrace {

struct Ray;
class Texture : public SceneElement, public HeapAlignedType<Vector3f> {
public:
	/**
	 * Evaluate the texture at a given point.
	 * @param ray The ray we are tracing
	 * @param uv uv mapped coordinate
	 * @param p The coordinate of the intersaction
	 * @return The value of the texture at the point
	 */
	virtual Color3f getValue(const Ray& ray, const Vector2f& uv, const Vector3f& p) const = 0;
};

/**
 * Evaluate a textute that can also be nullptr.
 * If the texture wasn't specified in the scene file -> return DEFAULT_TEXTURE_VALUE
 */
inline Vector3f getTextureValue(const Texture* text, const Ray& ray, const Vector2f& uv, const Vector3f& p) {
	return (text == nullptr) ?
		DEFAULT_TEXTURE_VALUE :
		text->getValue(ray, uv, p);
}
}
