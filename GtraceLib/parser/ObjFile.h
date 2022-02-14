#pragma once
#include <vector>
#include <stdint.h>
#include <string>

namespace gtrace
{
class Vector3f;
class ObjFile
{
public:
    struct Indices
    {
        uint32_t i, j, k;
    };
    struct RawMesh
    {
        std::vector<Vector3f> vertices;
        std::vector<Vector3f> normals;
        std::vector<Indices> faces;
        std::vector<Indices> facesNormals;
    };

    static RawMesh parse(const char* filePath);

protected:
    static inline const std::string comment = "#";
    static constexpr char delimer = ' ';
    static inline const std::string vertex = "v";
};
}  // namespace gtrace