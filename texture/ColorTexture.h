#pragma once

#include "Texture.h"

class ColorTexture : public Texture {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override {
		SceneParser& p = getParser();
		Vector3f v;
		p.parseVector3fAndStore(map, "color", this->color);
	}

	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const override {
		return Vector3f(color);
	}
private:
	Vector3fData color;
};