#include "MatteMaterial.h"

#include "../scene/World.h"
#include "../reflection/Lambertian.h"

void MatteMaterial::parse(std::unordered_map<std::string, std::string>& map) {
	SceneParser::parseTextureAndStore(map, "text", this->text);
	SceneParser::parsefloatAndStore(map, "k", this->k);
}

BSDF MatteMaterial::getBSDF(const Ray& ray, const HitRecord& hr) const {
	BSDF res(ray, hr);
	const Vector3f r = getTextureValue(this->text, Vector2f(), hr.position);
	res.add(std::make_unique<LambertianReflection>(r));
	return res;
}
