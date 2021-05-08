#pragma once

#include "Material.h"
#include "texture/Texture.h"
#include "reflection/Lambertian.h"

namespace gtrace {

class MatteMaterial : public Material {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;
	virtual BSDF getBSDF(const Ray& ray, const HitRecord& hr) const override;
private:
	Texture* text;
	LambertianReflection lambertian;
};
}
