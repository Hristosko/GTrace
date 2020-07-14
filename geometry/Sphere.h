#pragma once

#include "Shape.h"

class Sphere : public Shape {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
	virtual BBox bbox() const override;

private:
	Vector3f center;
	float radius;
	Material* mat;
};
