#pragma once

#include "Shape.h"

class Sphere : public Shape {
public:
	void parse(std::unordered_map<std::string, std::string>& map) override;
	bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
private:
	Vector3fData center;
	float radius;
	Material* mat;
};
