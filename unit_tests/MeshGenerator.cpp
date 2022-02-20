#include <unordered_map>
#include "MeshGenerator.h"
#include "math/Vector3f.h"

struct Edge
{
    uint32_t i, j;

    bool operator==(const Edge& rhs) const { return i == rhs.i && j == rhs.j; }
};

namespace std
{
template<>
struct hash<Edge>
{
    size_t operator()(const Edge& edge) const
    {
        uint64_t num = reinterpret_cast<const uint64_t&>(edge);
        return hash<uint64_t>()(num);
    };
};
}  // namespace std

using EdgeMap = std::unordered_map<Edge, uint32_t>;

static void addTriangle(RawMesh* mesh, uint32_t i, uint32_t j, uint32_t k)
{
    mesh->faces.emplace_back(MeshTriangle{i, j, k});
}

static RawMesh icosahedron()
{
    RawMesh mesh;
    const float X = .525731112119133606f;
    const float Z = .850650808352039932f;
    const float N = 0.f;

    mesh.vertices = {{-X, N, Z}, {X, N, Z},   {-X, N, -Z}, {X, N, -Z}, {N, Z, X},  {N, Z, -X},
                     {N, -Z, X}, {N, -Z, -X}, {Z, X, N},   {-Z, X, N}, {Z, -X, N}, {-Z, -X, N}};

    mesh.faces = {{0, 4, 1}, {0, 9, 4},  {9, 5, 4},  {4, 5, 8},  {4, 8, 1},  {8, 10, 1}, {8, 3, 10},
                  {5, 3, 8}, {5, 2, 3},  {2, 7, 3},  {7, 10, 3}, {7, 6, 10}, {7, 11, 6}, {11, 0, 6},
                  {0, 1, 6}, {6, 1, 10}, {9, 0, 11}, {9, 11, 2}, {9, 2, 5},  {7, 2, 11}};
    return mesh;
}

static uint32_t subdivideEdge(
    uint32_t f0,
    uint32_t f1,
    const Vector3f& v0,
    const Vector3f& v1,
    RawMesh* mesh,
    EdgeMap* subdivisions)
{
    if (f1 < f0)
        std::swap(f0, f1);

    const Edge edge{f0, f1};
    const auto it = subdivisions->find(edge);
    if (it != subdivisions->end())
        return it->second;

    const auto v = normalize(v0 + v1);
    const uint32_t f = mesh->vertices.size();
    mesh->vertices.emplace_back(v);
    subdivisions->emplace(edge, f);
    return f;
}

static RawMesh subdivideMesh(const RawMesh& mesh)
{
    EdgeMap subdivisions;
    RawMesh res;
    res.vertices = mesh.vertices;
    for (const auto& triangle : mesh.faces)
    {
        const uint32_t f0 = triangle.i;
        const uint32_t f1 = triangle.j;
        const uint32_t f2 = triangle.k;

        const Vector3f v0 = mesh.vertices[f0];
        const Vector3f v1 = mesh.vertices[f1];
        const Vector3f v2 = mesh.vertices[f2];

        const uint32_t f3 = subdivideEdge(f0, f1, v0, v1, &res, &subdivisions);
        const uint32_t f4 = subdivideEdge(f1, f2, v1, v2, &res, &subdivisions);
        const uint32_t f5 = subdivideEdge(f2, f0, v2, v0, &res, &subdivisions);

        addTriangle(&res, f0, f3, f5);
        addTriangle(&res, f3, f1, f4);
        addTriangle(&res, f4, f2, f5);
        addTriangle(&res, f3, f4, f5);
    }

    return res;
}

RawMesh generateMeshSphere(int subdiveCount, UseNormals genNormals)
{
    RawMesh mesh = icosahedron();
    for (int i = 0; i < subdiveCount; ++i)
        mesh = subdivideMesh(mesh);

    if (genNormals == UseNormals::True)
        generateMeshSohereNormals(&mesh);

    return mesh;
}

void generateMeshSohereNormals(RawMesh* mesh)
{
    mesh->normals = mesh->vertices;
    mesh->facesNormals = mesh->faces;
}
