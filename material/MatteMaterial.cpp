#include "MatteMaterial.h"

void MatteMaterial::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser& p = getParser();
	p.parseTextureAndStore(map, "text", this->text);
	p.parsefloatAndStore(map, "k", this->k);
}

Vector3f MatteMaterial::shade(const HitRecord& rec, const Ray& ray) const {
	const Vector3f color =
		(this->text == nullptr) ? 
		DEFAULT_TEXTURE_VALUE :
		this->text->getValue(Vector2f(), rec.position);
	return color;
}
