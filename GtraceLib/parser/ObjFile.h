#pragma once
#include <vector>
#include <stdint.h>
#include <string>
#include "geometry/MeshData.h"

namespace gtrace
{
class Vector3f;
class ParserContext;
class ObjFile
{
public:
    static RawMesh parse(const char* filePath);
    static void dump(const RawMesh& mesh, const char* filePath);

protected:
    static void parseFace(RawMesh* mesh, const std::vector<std::string_view>& fields, const ParserContext& context);
    static void parseIndices(
        std::string_view str,
        const ParserContext& context,
        const RawMesh& mesh,
        uint32_t* v,
        uint32_t* n);

protected:
    static inline const std::string comment = "#";
    static constexpr char delimer = ' ';
    static inline const std::string vertex = "v";
    static inline const std::string normal = "vn";
    static inline const std::string face = "f";
    static inline const std::string faceNormalDelimer = "//";
};
}  // namespace gtrace