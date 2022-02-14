#include <cstdlib>
#include "ObjFile.h"
#include "FileReader.h"
#include "StringUtils.h"
#include "common/Errors.h"
#include "common/Logger.h"
#include "math/Vector3f.h"

namespace gtrace
{
static float parseFloat(std::string_view str)
{
    char* end;
    float res = std::strtof(str.data(), &end);
    if (end != str.data() + str.size())
        Raise(ParserError(std::string("Unable to parse float from: ") + str.data()));
    return res;
}

static Vector3f parseVector3f(const std::vector<std::string_view>& fields)
{
    return Vector3f(parseFloat(fields[1]), parseFloat(fields[2]), parseFloat(fields[3]));
}

ObjFile::RawMesh ObjFile::parse(const char* filePath)
{
    FileReader reader(filePath);
    LOGINFO("Start parsing obj file: ", filePath);

    RawMesh mesh;
    std::string line;
    while (reader.readLine(&line))
    {
        if (line.empty() || startsWith(line, comment))
            continue;

        const auto fields = split(line, delimer);
        if (fields[0] == vertex)
            mesh.vertices.emplace_back(parseVector3f(fields));
    }

    return mesh;
}

}  // namespace gtrace