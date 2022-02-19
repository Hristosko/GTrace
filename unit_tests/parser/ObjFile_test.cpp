#include <stdio.h>
#include "TestUtils.h"
#include "MeshGenerator.h"
#include "common/Errors.h"
#include "parser/FileWriter.h"
#include "parser/ObjFile.h"

using namespace gtrace;

class ObjFileTest : public Test
{
public:
    static inline const std::string testFilesDirPath = "unit_tests/parser/test_files/";
    static constexpr auto* file = "test.obj";

    void SetUp() override { writer = FileWriter(file); }

    void TearDown() override
    {
        writer.close();
        remove(file);
    }

    void writeVertex() const { writer.writeLine("v 0 0 0"); }

    void writeNormal() const { writer.writeLine("vn 0 0 0"); }

    FileWriter writer;
};

namespace gtrace
{
bool operator==(const MeshTriangle& a, const MeshTriangle& b)
{
    return a.i == b.i && a.j == b.j && a.k == b.k;
}

bool operator!=(const MeshTriangle& a, const MeshTriangle& b)
{
    return !(a == b);
}

bool operator==(const RawMesh& a, const RawMesh& b)
{
    return a.vertices == b.vertices && a.faces == b.faces && a.normals == b.normals && a.facesNormals == b.facesNormals;
}
}  // namespace gtrace

#ifndef WIN32

TEST_F(ObjFileTest, SimpleObjFileParse)
{
    const auto res = ObjFile::parse((testFilesDirPath + "obj_file.obj").c_str());

    ASSERT_EQ(1, res.vertices.size());
    ASSERT_EQ(Vector3f(0.1102022, 0.74011, 1.132398), res.vertices[0]);

    ASSERT_EQ(1, res.normals.size());
    ASSERT_EQ(Vector3f(-0.136243, 0.973867, -0.181718), res.normals[0]);

    ASSERT_EQ(2, res.faces.size());
    ASSERT_EQ(1, res.facesNormals.size());

    ASSERT_EQ(0, res.faces[0].i);
    ASSERT_EQ(0, res.faces[0].j);
    ASSERT_EQ(0, res.faces[0].k);

    ASSERT_EQ(0, res.faces[1].i);
    ASSERT_EQ(0, res.faces[1].j);
    ASSERT_EQ(0, res.faces[1].k);

    ASSERT_EQ(0, res.facesNormals[0].i);
    ASSERT_EQ(0, res.facesNormals[0].j);
    ASSERT_EQ(0, res.facesNormals[0].k);
}

TEST_F(ObjFileTest, ParseAndDumpObjFile)
{
    writer.close();
    const auto parsed = ObjFile::parse((testFilesDirPath + "obj_file.obj").c_str());
    ObjFile::dump(parsed, file);
    const auto mesh = ObjFile::parse(file);

    ASSERT_TRUE(compare(parsed.vertices, mesh.vertices));
    ASSERT_TRUE(compare(parsed.normals, mesh.normals));
    ASSERT_TRUE(compare(parsed.faces, mesh.faces));
    ASSERT_TRUE(compare(parsed.facesNormals, mesh.facesNormals));
}

#endif  // !WIN32

TEST_F(ObjFileTest, InvalidVertex)
{
    writer.writeLine("v 1 2 3c");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, VertexTooManyValues)
{
    writer.writeLine("v 1 2 3 4");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, VertexTooFewValues)
{
    writer.writeLine("v 1 2");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, UnknownField)
{
    writer.writeLine("XXX 1 2 3");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, NegativeIndex)
{
    writeVertex();
    writeNormal();
    writer.writeLine("f 1 1 -1");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, ReferenceUnexisingVertex)
{
    writeVertex();
    writeNormal();
    writer.writeLine("f 1 2 1");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, ReferenceUnexisingNormal)
{
    writeVertex();
    writeNormal();
    writer.writeLine("f 1//1 1//1 1//2");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, TooManyIndices)
{
    writeVertex();
    writeNormal();
    writer.writeLine("f 1 1 1 1");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, TooFewIndices)
{
    writeVertex();
    writeNormal();
    writer.writeLine("f 1 1");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, TooManyArgumentsPerIndex)
{
    writeVertex();
    writeNormal();
    writer.writeLine("f 1//1 1//1 1//1//1");
    writer.close();
    EXPECT_THROW(ObjFile::parse(file), ParserError);
}

TEST_F(ObjFileTest, SphereMeshsDumpAndParse)
{
    writer.close();
    const RawMesh mesh = generateMeshSphere(3);
    ObjFile::dump(mesh, file);
    const RawMesh parsedMesh = ObjFile::parse(file);
    ASSERT_EQ(mesh, parsedMesh);
}
