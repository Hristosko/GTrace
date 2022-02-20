#pragma once
#include "MeshData.h"
#include "Shape.h"

namespace gtrace
{
class BVH;
class Triangle;

class Mesh
{
public:
    friend class MeshElement;
    friend class MeshElementWithNormal;

    Mesh(const ParsedParams& params, BVH* bvh);

private:
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
};

class MeshElement : public Shape
{
public:
    MeshElement(const ParsedParams& params, const Mesh* mesh, const MeshTriangle& faces);

    bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const override;
    BBox bound() const override;

protected:
    Triangle makeTriangle() const;

protected:
    const Mesh* mesh;
    MeshTriangle faces;
};

class MeshElementWithNormal : public MeshElement
{
public:
    MeshElementWithNormal(
        const ParsedParams& params,
        const Mesh* mesh,
        const MeshTriangle& faces,
        const MeshTriangle& normals);

    bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const override;

protected:
    MeshTriangle normals;
};

}  // namespace gtrace