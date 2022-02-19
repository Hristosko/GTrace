#include "Mesh.h"
#include "BBox.h"
#include "BVH.h"
#include "Triangle.h"
#include "math/Transform.h"
#include "parser/ObjFile.h"
#include <iostream>
namespace gtrace
{
Mesh::Mesh(const ParsedParams& params, BVH* bvh)
{
    const auto& modelFilePath = params.getString("model");
    const RawMesh rawMesh = ObjFile::parse(modelFilePath.c_str());
    vertices = std::move(rawMesh.vertices);
    normals = std::move(rawMesh.normals);
    vertices.shrink_to_fit();
    normals.shrink_to_fit();

    for (size_t i = 0; i < rawMesh.faces.size(); ++i)
    {
        bvh->insert(std::make_unique<MeshElement>(params, this, rawMesh.faces[i]));
    }
}

MeshElement::MeshElement(const ParsedParams& params, const Mesh* mesh, const MeshTriangle& faces) :
    Shape(params),
    mesh(mesh),
    faces(faces)
{
}

bool MeshElement::hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const
{
    float beta, gamma, tval;
    const auto tr = makeTriangle();
    if (tr.hit(ray, tmin, tmax, &beta, &gamma, &tval))
    {
        interection->normal = objectToWorld->transformDirection(normalize(cross((tr.b - tr.a), (tr.a - tr.c))));
        // if (dot(interection->normal, ray.origin) < 0.f)
        //    interection->normal *= -1.f;
        interection->set(tval, this);
        return true;
    }
    return false;
}

BBox MeshElement::bound() const
{
    return makeTriangle().bound();
}

Triangle MeshElement::makeTriangle() const
{
    return Triangle(mesh->vertices[faces.i], mesh->vertices[faces.j], mesh->vertices[faces.k], objectToWorld.get());
}

}  // namespace gtrace
