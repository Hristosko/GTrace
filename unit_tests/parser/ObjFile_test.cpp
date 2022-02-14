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
}

#endif  // !WIN32