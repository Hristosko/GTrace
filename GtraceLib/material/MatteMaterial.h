#pragma once

#include "Material.h"
#include "../texture/Texture.h"

class MatteMaterial : public Material {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;
	virtual Vector3f shade(const HitRecord& rec, const Ray& ray) const override;
	virtual BSDF getBSDF(const Ray& ray, const HitRecord& hr) const override;
private:
	Texture* text;
	float k;
};