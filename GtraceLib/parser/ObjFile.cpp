#include "ObjFile.h"
#include "FileReader.h"
#include "FileWriter.h"
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

static uint32_t getInvalidIndex()
{
    return std::numeric_limits<uint32_t>::max();
}

static bool isValidIndex(uint32_t idx)
{
    return idx != getInvalidIndex();
}

void ObjFile::parseIndices(
    std::string_view str,
    const ParserContext& context,
    const RawMesh& mesh,
    uint32_t* v,
    uint32_t* n)
{
    *n = getInvalidIndex();
    const auto tokens = ParsingStrings::split(str, faceNormalDelimer);
    if (tokens.size() < 1 || tokens.size() > 2)
        Raise(ParserError(context.message() + "Unexpected number of values"));

    const auto vv = ParsingStrings::parseNumber(tokens[0], context);

    if (vv <= 0 || vv > mesh.vertices.size())
        Raise(ParserError(context.message() + "Invalid face index: " + str.data()));

    *v = static_cast<uint32_t>(vv - 1);

    if (tokens.size() == 1)
        return;

    const auto nn = ParsingStrings::parseNumber(tokens[1], context);
    if (nn <= 0 || nn > mesh.normals.size())
        Raise(ParserError(context.message() + "Invalid normal index: " + str.data()));

    *n = static_cast<uint32_t>(nn - 1);
}

void ObjFile::parseFace(RawMesh* mesh, const std::vector<std::string_view>& fields, const ParserContext& context)
{
    if (fields.size() != 4)
        Raise(ParserError(context.message() + "Unexpected number of values"));

    MeshTriangle vers, norms;
    parseIndices(fields[1], context, *mesh, &vers.i, &norms.i);
    parseIndices(fields[2], context, *mesh, &vers.j, &norms.j);
    parseIndices(fields[3], context, *mesh, &vers.k, &norms.k);

    mesh->faces.emplace_back(vers);
    if (isValidIndex(norms.i) || isValidIndex(norms.j) || isValidIndex(norms.k))
        mesh->facesNormals.emplace_back(norms);
}

RawMesh ObjFile::parse(const char* filePath)
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
        else
            Raise(ParserError(context.message() + "Unknown obj file filed: " + fields[0].data()));
    }

    LOGINFO("Parsed obj file: ", filePath);
    LOGINFO("Vertecies: ", mesh.vertices.size());
    LOGINFO("Normals: ", mesh.normals.size());
    LOGINFO("Faces: ", mesh.faces.size());
    LOGINFO("Faces Normals: ", mesh.facesNormals.size());
    return mesh;
}

static std::string toString(const Vector3f& v)
{
    std::string res;
    for (auto i = 0; i < 3; ++i)
    {
        res += std::to_string(v[i]);
        if (i != 2)
            res += ' ';
    }
    return res;
}

static std::string toString(uint32_t face, uint32_t normal, bool hasNormal, const std::string& delimer)
{
    auto res = std::to_string(face);
    if (hasNormal)
        res += delimer + std::to_string(normal);
    return res;
}

void ObjFile::dump(const RawMesh& mesh, const char* filePath)
{
    FileWriter writer(filePath);
    LOGINFO("Writing obj file: ", filePath);

    for (const auto& v : mesh.vertices)
        writer.writeLine(vertex + delimer + toString(v));

    for (const auto& n : mesh.normals)
        writer.writeLine(normal + delimer + toString(n));

    for (std::vector<MeshTriangle>::size_type i = 0; i < mesh.faces.size(); ++i)
    {
        const auto faces = mesh.faces[i];
        const auto hasNormal = i < mesh.facesNormals.size();
        const auto normals = hasNormal ? mesh.facesNormals[i] : MeshTriangle();

        const auto indexToString = [hasNormal](uint32_t face, uint32_t normal) {
            return toString(face + 1, normal + 1, hasNormal, faceNormalDelimer);
        };

        const std::string res = face + delimer + indexToString(faces.i, normals.i) + delimer +
                                indexToString(faces.j, normals.j) + delimer + indexToString(faces.k, normals.k);
        writer.writeLine(res);
    }
    LOGINFO("Finish writing obj file: ", filePath);
}

}  // namespace gtrace