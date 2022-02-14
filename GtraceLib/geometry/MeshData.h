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

class MeshData
{
public:
    using const_iterator = std::vector<Vector3f>::const_iterator;
    MeshData(std::vector<Vector3f>&& vertices, std::vector<Vector3f>&& normals);

    const_iterator verticesBegin() const { return vertices.cbegin(); }
    const_iterator verticesEnd() const { return vertices.cend(); }

    const_iterator normalsBegin() const { return normals.cbegin(); }
    const_iterator normalsEnd() const { return normals.cend(); }

private:
    std::vector<Vector3f> vertices;
    std::vector<Vector3f> normals;
};

}  // namespace gtrace
