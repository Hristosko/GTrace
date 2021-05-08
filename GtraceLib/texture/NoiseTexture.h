#pragma once

#include "Texture.h"
#include "SolidNoise.h"

namespace gtrace {

class NoiseTexture : public Texture {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

	virtual Color3f getValue(const Ray& ray, const Vector2f& uv, const Vector3f& p) const override;
private:
	float scale;
	Texture* text1; /** The first texture part of the noise */
	Texture* text2; /** The second texture part of the noise */
	SolidNoise noise; /** Solid noise use to generate specific noise (perlin noise) */
};
}
