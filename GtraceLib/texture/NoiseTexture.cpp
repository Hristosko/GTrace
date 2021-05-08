#include "NoiseTexture.h"

#define DEFAULT_SCALE_VALUE 0.1f

void NoiseTexture::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	this->scale = DEFAULT_SCALE_VALUE;
	parser.parsefloatAndStore(map, "scale", this->scale);
	parser.parseTextureAndStore(map, "text1", this->text1);
	parser.parseTextureAndStore(map, "text2", this->text2);
}

Vector3f NoiseTexture::getValue(const Vector2f& uv, const Vector3f& p) const {
	const float t = (1.f + this->noise.perlinNoise(p * this->scale)) * 0.5f;
	const Vector3f c1 = getTextureValue(this->text1, uv, p);
	const Vector3f c2 = getTextureValue(this->text2, uv, p);
	return t * c1 + (1.f - t) * c2;
}
