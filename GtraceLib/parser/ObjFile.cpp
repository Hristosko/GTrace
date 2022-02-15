#include "ObjFile.h"
#include "FileReader.h"
#include "ParsingStrings.h"
#include "common/Errors.h"
#include "common/Logger.h"
#include "math/Vector3f.h"

namespace gtrace
{
static Vector3f parseVector3f(const std::vector<std::string_view>& fields, const ParserContext& context)
{
    return Vector3f(
        ParsingStrings::parseFloat(fields[1], context), ParsingStrings::parseFloat(fields[2], context),
        ParsingStrings::parseFloat(fields[3], context));
}

ObjFile::RawMesh ObjFile::parse(const char* filePath)
{
    ParserContext context {filePath, 0};
    FileReader reader(filePath);
    LOGINFO("Start parsing obj file: ", filePath);

    RawMesh mesh;
    std::string line;
    while (reader.readLine(&line))
    {
        ++context.line;

        if (line.empty() || ParsingStrings::startsWith(line, comment))
            continue;

        const auto fields = ParsingStrings::split(line, delimer);
        if (fields[0] == vertex)
            mesh.vertices.emplace_back(parseVector3f(fields, context));
    }

    return mesh;
}

}  // namespace gtrace