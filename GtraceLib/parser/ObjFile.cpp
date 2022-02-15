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
    if (fields.size() != 4)
        Raise(ParserError(context.message() + "Unexpected number of values"));
    return Vector3f(
        ParsingStrings::parseFloat(fields[1], context), ParsingStrings::parseFloat(fields[2], context),
        ParsingStrings::parseFloat(fields[3], context));
}

void ObjFile::parseIndices(
    std::string_view str,
    const ParserContext& context,
    const RawMesh& mesh,
    uint32_t* v,
    uint32_t* n)
{
    const auto tokens = ParsingStrings::split(str, faceNormalDelimer);
    if (tokens.size() != 2)
        Raise(ParserError(context.message() + "Unexpected number of values"));

    const auto vv = ParsingStrings::parseNumber(tokens[0], context);
    const auto nn = ParsingStrings::parseNumber(tokens[1], context);

    if (vv <= 0 || nn <=0 || vv > mesh.vertices.size() || nn > mesh.normals.size())
        Raise(ParserError(context.message() + "Invalid face/normal index: " + str.data()));

    *v = static_cast<uint32_t>(vv);
    *n = static_cast<uint32_t>(nn);
}

void ObjFile::parseFace(RawMesh* mesh, const std::vector<std::string_view>& fields, const ParserContext& context)
{
    if (fields.size() != 4)
        Raise(ParserError(context.message() + "Unexpected number of values"));

    Indices vers, norms;
    parseIndices(fields[1], context, *mesh, &vers.i, &norms.i);
    parseIndices(fields[2], context, *mesh, &vers.j, &norms.j);
    parseIndices(fields[3], context, *mesh, &vers.k, &norms.k);

    mesh->faces.emplace_back(vers);
    mesh->facesNormals.emplace_back(norms);
}

ObjFile::RawMesh ObjFile::parse(const char* filePath)
{
    ParserContext context{filePath, 0};
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
        else if (fields[0] == normal)
            mesh.normals.emplace_back(parseVector3f(fields, context));
        else if (fields[0] == face)
            parseFace(&mesh, fields, context);
    }

    return mesh;
}

}  // namespace gtrace