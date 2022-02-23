#include "Mesh.h"
#include "BBox.h"
#include "BVH.h"
#include "Triangle.h"
#include "common/Logger.h"
#include "math/Transform.h"
#include "parser/ObjFile.h"

namespace gtrace
{
void Mesh::build(const ParsedParams& params, BVH* bvh)
{
    const auto& modelFilePath = params.getString("model");
    const RawMesh rawMesh = ObjFile::parse(modelFilePath.c_str());
    vertices = std::move(rawMesh.vertices);
    normals = std::move(rawMesh.normals);
    vertices.shrink_to_fit();
    normals.shrink_to_fit();

    if (rawMesh.facesNormals.size() > 0)
        LOGINFO("Crating mesh with normals.")
    else
        LOGINFO("Crating mesh without normals.")

    for (size_t i = 0; i < rawMesh.faces.size(); ++i)
    {
        if (i < rawMesh.facesNormals.size())
            bvh->insert(std::make_unique<MeshElementWithNormal>(
                params, shared_from_this(), rawMesh.faces[i], rawMesh.facesNormals[i]));
        else
            bvh->insert(std::make_unique<MeshElement>(params, shared_from_this(), rawMesh.faces[i]));
    }
}

MeshElement::MeshElement(const ParsedParams& params, std::shared_ptr<const Mesh> mesh, const MeshTriangle& faces) :
    Shape(params),
    mesh(std::move(mesh)),
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

MeshElementWithNormal::MeshElementWithNormal(
    const ParsedParams& params,
    std::shared_ptr<const Mesh> mesh,
    const MeshTriangle& faces,
    const MeshTriangle& normals) :
    MeshElement(params, std::move(mesh), faces),
    normals(normals)
{
}

bool MeshElementWithNormal::hit(const Ray& ray, float tmin, float tmax, float time, Intersection* interection) const
{
    float beta, gamma, tval;
    const auto tr = makeTriangle();
    if (tr.hit(ray, tmin, tmax, &beta, &gamma, &tval))
    {
        const float alpha = 1.f - beta - gamma;
        interection->normal =
            alpha * mesh->normals[normals.i] + beta * mesh->normals[normals.j] + gamma * mesh->normals[normals.k];
        interection->set(tval, this);
        return true;
    }
    return false;
}

}  // namespace gtrace
