#pragma once

#include "Light.h"
#include "../math/Vector2f.h"

class DirectionalLight : public Light {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;
	virtual void radiance(Vector3f& direction, Vector3f& color) const override;
	virtual Vector3f sample(const HitRecord& hr, const Point2f& u, Vector3f& wi, float& pdf) const override;

private:
	Vector3f direction;
	Vector3f color;
};