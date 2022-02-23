#include "TestUtils.h"
#include "MeshGenerator.h"
#include "geometry/Mesh.h"
#include "geometry/BBox.h"
#include "geometry/BVH.h"
#include "parser/ObjFile.h"
#include "parser/ParsedParams.h"

using namespace gtrace;

static inline const auto meshNormalsTestParams = ::testing::Values(UseNormals::False, UseNormals::True);

class MeshTest : public ::testing::TestWithParam<UseNormals>
{
public:
    static inline const char* modelWithoutNormals = "model_without_normals.obj";
    static inline const char* modelWithNormals = "model_with_normals.obj";

    static void SetUpTestCase()
    {
        auto rawMesh = generateMeshSphere(5, UseNormals::False);
        ObjFile::dump(rawMesh, modelWithoutNormals);

        generateMeshSohereNormals(&rawMesh);
        ObjFile::dump(rawMesh, modelWithNormals);
    }

    static void TearDownTestCase()
    {
        remove(modelWithoutNormals);
        remove(modelWithNormals);
    }

    void SetUp() override
    {
        MemoryTracker::reset();
        mesh = std::make_shared<Mesh>();
        params = std::make_unique<ParsedParams>();
        bvh = std::make_unique<BVH>();

        if (GetParam() == UseNormals::False)
            params->addString("model", modelWithoutNormals);
        else
            params->addString("model", modelWithNormals);
    }

    void TearDown() override
    {
        mesh.reset();
        params.reset();
        bvh.reset();
        checkMemoryLeaks();
    }

    std::shared_ptr<Mesh> mesh;
    std::unique_ptr<ParsedParams> params;
    std::unique_ptr<BVH> bvh;
};

TEST_P(MeshTest, noTransform)
{
    mesh->build(*params, bvh.get());
    mesh.reset();
    bvh->build();

    {
        const Ray ray(Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.x(), 1.f, EPS);
    }
    {
        const Ray ray(Vector3f(0.f, 0.f, -2.f), Vector3f(0.f, 0.f, 1.f));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.z(), -1.f, EPS);
    }
    {
        const Ray ray(Vector3f(0.f, -2.f, 0.f), Vector3f(0.f, 1.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.y(), -1.f, EPS);
    }
    {
        const Vector3f origin = Vector3f(1.f) + randomVector3f();
        const Vector3f target = Vector3f(0.f);
        const Ray ray(origin, normalize(target - origin));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Vector3f origin = Vector3f(1.f) + randomVector3f();
        const Vector3f target = Vector3f(0.f);
        const Ray ray(origin, -normalize(target - origin));
        Intersection intersection;
        ASSERT_FALSE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Ray ray(Vector3f(2.f, 0.0f, 0.0f), Vector3f(1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_FALSE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Ray ray(Vector3f(2.f, 0.0f, 20.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_FALSE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
}

TEST_P(MeshTest, translation)
{
    const Vector3f center(100.f);
    params->addTranslation(center);
    mesh->build(*params, bvh.get());
    mesh.reset();
    bvh->build();

    {
        const Ray ray(center + Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.x(), 1.f, EPS);
    }
    {
        const Ray ray(center + Vector3f(0.f, 0.f, -2.f), Vector3f(0.f, 0.f, 1.f));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.z(), -1.f, EPS);
    }
    {
        const Ray ray(center + Vector3f(0.f, -2.f, 0.f), Vector3f(0.f, 1.f, 0.f));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_NEAR(intersection.time, 1.f, EPS);
        EXPECT_NEAR(intersection.normal.y(), -1.f, EPS);
    }
    {
        const Vector3f origin = center + Vector3f(2.f) + randomVector3f();
        const Vector3f target = center;
        const Ray ray(origin, normalize(target - origin));
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
    {
        const Ray ray(Vector3f(2.f, 0.0f, 0.0f), Vector3f(-1.f, 0.f, 0.f));
        Intersection intersection;
        ASSERT_FALSE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
    }
}

TEST_P(MeshTest, scale)
{
    const float R = 10.f;
    params->addScale(R);
    mesh->build(*params, bvh.get());
    mesh.reset();
    bvh->build();

    {
        const Vector3f distance = normalize(Vector3f(1.f));
        const Ray ray(Vector3f(R) + distance, -distance);
        Intersection intersection;
        ASSERT_TRUE(bvh->hit(ray, 0.f, 1000.f, 0.f, &intersection));
        EXPECT_EQ(intersection.normal, distance);
        intersection.point = ray.origin + intersection.time * ray.direction;
        ASSERT_NEAR(intersection.point.length(), R, 0.005f);
    }
}

INSTANTIATE_TEST_CASE_P(MeshTests, MeshTest, meshNormalsTestParams);
