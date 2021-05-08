#pragma once

#include "Texture.h"

namespace gtrace {

class ColorTexture : public Texture {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override {
		Vector3f v;
		parser.parseVector3fAndStore(map, "color", this->color);
	}

	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const override {
		return Vector3f(color);
	}
private:
	Vector3f color;
};
}
