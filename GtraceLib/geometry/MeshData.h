#pragma once

#include <vector>
#include <stdint.h>

namespace gtrace
{
class Vector3f;

/**
 * Stores the indexes of the nodes of the mesh.
 */
struct MeshTriangle
{
    uint32_t i, j, k;
};

struct RawMesh
{
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
    std::vector<MeshTriangle> faces;
    std::vector<MeshTriangle> facesNormals;
};

}  // namespace gtrace
