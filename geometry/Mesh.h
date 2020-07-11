#pragma once

#include "../math/Vector3f.h"
#include "../material/Material.h"
#include "Shape.h"
#include <deque>

struct MeshTriangle {
	uint32_t i, j, k;
};

class Mesh : public Shape {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;

private:
	void loadFromObjFile(const char* path, bool useNormals);
private:
	Material* mat;
	std::deque<Vector3fData> vertices;
	std::deque<Vector3fData> normals;
	std::deque<MeshTriangle> faces;
	std::deque<MeshTriangle> facesNormals;
};