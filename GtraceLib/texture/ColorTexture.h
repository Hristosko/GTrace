#pragma once

#include "Texture.h"

namespace gtrace {

/**
 * Texture that returns a constant.
 */
class ColorTexture : public Texture {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override {
		Vector3f v;
		parser.parseVector3fAndStore(map, "color", this->color);
	}

	virtual Color3f getValue(const Ray& ray, const Vector2f& uv, const Vector3f& p) const override {
		return Color3f(color);
	}
private:
	Color3f color;
};
}
