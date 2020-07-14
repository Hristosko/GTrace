#pragma once

#include "Light.h"

class DirectionalLight : public Light {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;
	virtual void radiance(Vector3f& direction, Vector3f& color) const override;
private:
	Vector3f direction;
	Vector3f color;
};