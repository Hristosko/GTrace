#pragma once

#include "Texture.h"
#include "SolidNoise.h"

namespace gtrace {

class NoiseTexture : public Texture {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

	virtual Vector3f getValue(const Vector2f& uv, const Vector3f& p) const override;
private:
	float scale;
	Texture* text1;
	Texture* text2;
	SolidNoise noise;
};
}
