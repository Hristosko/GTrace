#pragma once

#include "../math/Vector3f.h"
#include "../material/Material.h"
#include "Shape.h"
#include <deque>

namespace gtrace {

/**
 * Stores the indexes of the nodes of the mesh.
 */
struct MeshTriangle {
	uint32_t i, j, k;
};

/**
 * Stores the whole mesh. Each triangle is a separate elemnt that references the mesh.
 * The mesh stores the actual verteces.
 */
class Mesh : public SceneElement {
public:
	friend class MeshElement;
	friend class MeshElementWithNormal;
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;

private:
	void loadFromObjFile(World& w, const char* path, bool useNormals, Ref<Transform>& tr);
private:
	Material* mat; /** The material of the object */
	std::deque<Vector3f> vertices; /** The vertices */
	std::deque<Vector3f> normals; /** The normals (if any) */
};

/**
 * Part of the geometry. References vertices in a Mesh.
 */
class MeshElement : public Shape {
public:
	MeshElement(Ref<Transform>& tr, Mesh* mesh, uint32_t i, uint32_t j, uint32_t k)
		:Shape(tr), mesh(mesh), tr({ i, j, k }) {}

	// not used, cannot be instanciated from the parser
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override {}
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
	virtual BBox bbox() const override;

protected:
	Mesh* mesh;
	MeshTriangle tr;
};

/**
 * Part of the geometry. References vertices and normal in a Mesh.
 */
class MeshElementWithNormal : public MeshElement {
public:
	MeshElementWithNormal(Ref<Transform>& tr, Mesh* mesh, uint32_t i, uint32_t j, uint32_t k,
		uint32_t ni, uint32_t nj, uint32_t nk)
		: MeshElement(tr, mesh, i, j, k), normals({ ni, nj, nk }) {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;

protected:
	MeshTriangle normals;
};
}
