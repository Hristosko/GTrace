#pragma once

#include "../math/Vector3f.h"
#include "../material/Material.h"
#include "Shape.h"
#include <deque>

struct MeshTriangle {
	uint32_t i, j, k;
};

class Mesh : public SceneElement {
public:
	friend class MeshElement;
	friend class MeshElementWithNormal;
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;

private:
	void loadFromObjFile(const char* path, bool useNormals);
private:
	Material* mat;
	std::deque<Vector3fData> vertices;
	std::deque<Vector3fData> normals;
};

class MeshElement : public Shape {
public:
	MeshElement(Mesh* mesh, uint32_t i, uint32_t j, uint32_t k)
		: mesh(mesh), tr({ i, j, k }) {}

	// not used, cannot be instanciated from the parsr
	virtual void parse(std::unordered_map<std::string, std::string>& map) override {}
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
	virtual BBox bbox() const override;

protected:
	Mesh* mesh;
	MeshTriangle tr;
};

class MeshElementWithNormal : public MeshElement {
public:
	MeshElementWithNormal(Mesh* mesh, uint32_t i, uint32_t j, uint32_t k,
		uint32_t ni, uint32_t nj, uint32_t nk)
		: MeshElement(mesh, i, j, k), normals({ ni, nj, nk }) {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;

protected:
	MeshTriangle normals;
};