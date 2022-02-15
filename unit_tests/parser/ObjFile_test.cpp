#include "TestUtils.h"
#include "parser/ObjFile.h"

using namespace gtrace;

class ObjFileTest : public Test
{
public:
    static inline const std::string testFilesDirPath = "unit_tests/parser/test_files/";
};

#ifndef WIN32

TEST_F(ObjFileTest, SimpleObjFileParse)
{
    const auto res = ObjFile::parse((testFilesDirPath + "obj_file.obj").c_str());

    ASSERT_EQ(1, res.vertices.size());
    ASSERT_EQ(Vector3f(0.1102022, 0.74011, 1.132398), res.vertices[0]);

    ASSERT_EQ(1, res.normals.size());
    ASSERT_EQ(Vector3f(-0.136243, 0.973867, -0.181718), res.normals[0]);

    ASSERT_EQ(1, res.faces.size());
    ASSERT_EQ(1, res.facesNormals.size());

    ASSERT_EQ(1, res.faces[0].i);
    ASSERT_EQ(1, res.faces[0].j);
    ASSERT_EQ(1, res.faces[0].k);

    ASSERT_EQ(1, res.facesNormals[0].i);
    ASSERT_EQ(1, res.facesNormals[0].j);
    ASSERT_EQ(1, res.facesNormals[0].k);
}

#endif  // !WIN32