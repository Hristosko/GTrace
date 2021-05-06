#pragma once

#include "Texture.h"

class ColorTexture : public Texture {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override {
		Vector3f v;
		SceneParser::parseVector3fAndStore(map, "color", this->color);
	}

	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const override {
		return Vector3f(color);
	}
private:
	Vector3f color;
};