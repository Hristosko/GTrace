#pragma once

#include "Texture.h"

class ColorTexture : Texture {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override {
		SceneParser& p = getParser();
		p.parseVector3fAndStore(map, "color", this->color);
	}

	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const override {
		return this->color;
	}
private:
	Vector3f color;
};