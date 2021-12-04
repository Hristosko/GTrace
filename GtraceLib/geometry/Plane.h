#pragma once

#include "Shape.h"

namespace gtrace {

class Plane : public Shape {
public:
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
	virtual BBox bbox() const override;

private:
	float x, y; /** 0.5 the size of the plane (it isn't infinite) */
	Material* mat; /** The material of the sphere */
};
}