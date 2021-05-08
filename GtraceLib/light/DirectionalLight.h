#pragma once

#include "Light.h"
#include "../math/Vector2f.h"

namespace gtrace {

class DirectionalLight : public Light {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;
	virtual Vector3f sample(const HitRecord& hr, const Point2f& u, Vector3f& wi, float& pdf) const override;

private:
	Vector3f direction;
	Vector3f color;
};
}
