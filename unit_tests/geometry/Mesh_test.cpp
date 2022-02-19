#include "TestUtils.h"
#include "MeshGenerator.h"
#include "geometry/Mesh.h"
#include "geometry/BBox.h"
#include "geometry/BVH.h"
#include "parser/ObjFile.h"
#include "parser/ParsedParams.h"

using namespace gtrace;

class MeshTest : public Test
{
public:
    static inline const char* model = "model.obj";

    static void SetUpTestCase()
    {
        const auto rawMesh = generateMeshSphere(5);
        ObjFile::dump(rawMesh, model);
    }

    static void TearDownTestCase()
    {
        remove(model);
    }

    void SetUp() override { params.addString("model", model); }

    ParsedParams params;
    BVH bvh;
};

TEST_F(MeshTest, noTransform)
{
    Mesh mesh(params, &bvh);
    bvh.build();

    {
        const Ray ray(Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.x(), 1.f, EPS);
    }
    {
        const Ray ray(Vector3f(0.f, 0.f, -2.f), Vector3f(0.f, 0.f, 1.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.z(), -1.f, EPS);
    }
    {
        const Ray ray(Vector3f(0.f, -2.f, 0.f), Vector3f(0.f, 1.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.y(), -1.f, EPS);
    }
    {
        const Vector3f origin = Vector3f(1.f) + randomVector3f();
        const Vector3f target = Vector3f(0.f);
        const Ray ray(origin, normalize(target - origin));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Vector3f origin = Vector3f(1.f) + randomVector3f();
        const Vector3f target = Vector3f(0.f);
        const Ray ray(origin, -normalize(target - origin));
        Intersection intersection;
        ASSERT_FALSE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Ray ray(Vector3f(2.f, 0.0f, 0.0f), Vector3f(1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_FALSE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Ray ray(Vector3f(2.f, 0.0f, 20.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_FALSE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
}

TEST_F(MeshTest, translation)
{
    const Vector3f center(100.f);
    params.addTranslation(center);
    Mesh mesh(params, &bvh);
    bvh.build();

    {
        const Ray ray(center + Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.x(), 1.f, EPS);
    }
    {
        const Ray ray(center + Vector3f(0.f, 0.f, -2.f), Vector3f(0.f, 0.f, 1.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.z(), -1.f, EPS);
    }
    {
        const Ray ray(center + Vector3f(0.f, -2.f, 0.f), Vector3f(0.f, 1.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.y(), -1.f, EPS);
    }
    {
        const Vector3f origin = center + Vector3f(1.f) + randomVector3f();
        const Vector3f target = Vector3f(0.f);
        const Ray ray(origin, normalize(target - origin));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Ray ray(Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_FALSE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
}

TEST_F(MeshTest, scale)
{
    const float R = 20.f;
    params.addScale(R);
    Mesh mesh(params, &bvh);
    bvh.build();

    {
        const Ray ray(Vector3f(R) + Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh.hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.x(), 1.f, EPS);
    }
}
