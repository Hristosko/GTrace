#pragma once

#include "Shape.h"
#include "Mesh.h"

class Piramid : public Shape {
public:
	static constexpr int facesCount = 4;
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
private:
	Mesh mesh;
	MeshTriangle faces[facesCount];
};