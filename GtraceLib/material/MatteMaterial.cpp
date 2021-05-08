#include "MatteMaterial.h"

namespace gtrace {

void MatteMaterial::parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) {
	parser.parseTextureAndStore(map, "text", this->text);
}

BSDF MatteMaterial::getBSDF(const Ray& ray, const HitRecord& hr) const {
	const Color3f r = getTextureValue(this->text, ray, Vector2f(), hr.position);
	BSDF res(ray, hr, r);
	res.add(&this->lambertian);
	return res;
}
}
