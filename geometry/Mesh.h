#pragma once

#include "../math/Vector3f.h"
#include "../material/Material.h"
#include "../HeapAlocated.h"
#include "Shape.h"
#include <deque>

struct MeshTriangle {
	uint32_t i, j, k;
};

class Mesh : public SceneElement, public HeapAlocated {
public:
	friend class MeshElement;
	friend class MeshElementWithNormal;
	virtual void parse(std::unordered_map<std::string, std::string>& map) override;

private:
	void loadFromObjFile(const char* path, bool useNormals, Ref<Transform>& tr);
private:
	Material* mat;
	std::deque<Vector3f> vertices;
	std::deque<Vector3f> normals;
};

class MeshElement : public Shape {
public:
	MeshElement(Ref<Transform>& tr, Mesh* mesh, uint32_t i, uint32_t j, uint32_t k)
		:Shape(tr), mesh(mesh), tr({ i, j, k }) {}

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
	MeshElementWithNormal(Ref<Transform>& tr, Mesh* mesh, uint32_t i, uint32_t j, uint32_t k,
		uint32_t ni, uint32_t nj, uint32_t nk)
		: MeshElement(tr, mesh, i, j, k), normals({ ni, nj, nk }) {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;

protected:
	MeshTriangle normals;
};