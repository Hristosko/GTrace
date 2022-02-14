#include "MeshData.h"
#include "math/Vector3f.h"

namespace gtrace
{
MeshData::MeshData(std::vector<Vector3f>&& vertices, std::vector<Vector3f>&& normals) :
    vertices(move(vertices)),
    normals(move(normals))
{
    vertices.shrink_to_fit();
    normals.shrink_to_fit();
}
}  // namespace gtrace