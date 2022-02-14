#include <array>
#include "gtest/gtest.h"
#include "TestUtils.h"

#include "geometry/Triangle.h"
#include "geometry/BBox.h"

using namespace gtrace;

class TriangleTest : public Test
{
public:
    static const Transform identity;
    static float beta;
    static float gamma;
    static float tval;
};

const Transform TriangleTest::identity(Transform::makeIdentity());
float TriangleTest::beta;
float TriangleTest::gamma;
float TriangleTest::tval;

TEST_F(TriangleTest, intersect_axis)
{
    Triangle t(Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(-1.f, 0.f, 0.f), &identity);
    const Ray ray(Vector3f(0.f, 0.5f, 2.f), Vector3f(0.f, 0.f, -1.f));
    ASSERT_TRUE(t.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(t.hit(ray, 0.f, 1000.f, &beta, &gamma, &tval));
}

TEST_F(TriangleTest, outwards_ray)
{
    Triangle t(Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(-1.f, 0.f, 0.f), &identity);
    const Ray ray(Vector3f(0.f, 0.5f, 2.f), Vector3f(0.f, 0.f, 1.f));
    Intersection intersection;
    ASSERT_FALSE(t.hit(ray, 0.f, 1000.f, &beta, &gamma, &tval));
}

TEST_F(TriangleTest, random_ray_towards_the_center)
{
    Triangle t(Vector3f(1.f, -1.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(-1.f, -1.f, 0.f), &identity);
    const Vector3f origin = randomVector3f();
    const Vector3f target(0.f);
    const Vector3f direction = target - origin;
    const Ray ray(origin, normalize(direction));

    Intersection intersection;
    ASSERT_TRUE(t.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(t.hit(ray, 0.f, 1000.f, &beta, &gamma, &tval));
}

TEST_F(TriangleTest, bbox)
{
    std::array<Vector3f, 3> edges;
    for (auto& v : edges)
        v = randomVector3f();

    Triangle t(edges[0], edges[1], edges[2], &identity);
    const auto bbox = t.bbox();

    for (const auto& v : edges)
    {
        EXPECT_LE(v, bbox.max());
        EXPECT_GE(v, bbox.min());
    }
}
