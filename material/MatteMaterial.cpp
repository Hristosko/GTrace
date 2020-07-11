#include "MatteMaterial.h"

#include "../scene/World.h"

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
	const auto& lights = getWorld().getLights();
	const Vector3f wo = -ray.direction;
	Vector3f wi, lightColor;
	Vector3f res;
	for (Light* l : lights) {
		l->radiance(wi, lightColor);
		const float ndotwi = fabsf(dot(rec.normal, wi));
		res += k * color * lightColor * ndotwi;
	}
	return clamp(res, 0.f, 1.f);
}
