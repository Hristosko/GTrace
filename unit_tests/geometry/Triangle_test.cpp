#include <array>
#include "gtest/gtest.h"
#include "TestUtils.h"

#include "geometry/Triangle.h"
#include "geometry/BBox.h"

using namespace gtrace;

TEST(Triangle, intersect_axis)
{
    Triangle t(Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(-1.f, 0.f, 0.f));
    const Ray ray(Vector3f(0.f, 0.5f, 2.f), Vector3f(0.f, 0.f, -1.f));
    HitRecord rec;
    ASSERT_TRUE(t.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(t.hit(ray, 0.f, 1000.f, 0.f, rec));
}

TEST(Triangle, outwards_ray)
{
    Triangle t(Vector3f(1.f, 0.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(-1.f, 0.f, 0.f));
    const Ray ray(Vector3f(0.f, 0.5f, 2.f), Vector3f(0.f, 0.f, 1.f));
    HitRecord rec;
    ASSERT_FALSE(t.hit(ray, 0.f, 1000.f, 0.f, rec));
}

TEST(Triangle, random_ray_towards_the_center)
{
    Triangle t(Vector3f(1.f, -1.f, 0.f), Vector3f(0.f, 1.f, 0.f), Vector3f(-1.f, -1.f, 0.f));
    const Vector3f origin = randomVector3f();
    const Vector3f target(0.f);
    const Vector3f direction = target - origin;
    const Ray ray(origin, normalize(direction));

    HitRecord rec;
    ASSERT_TRUE(t.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(t.hit(ray, 0.f, 1000.f, 0.f, rec));
    ASSERT_NEAR(rec.normal.length(), 1.f, EPS);

    rec.position = ray.origin + rec.t * ray.direction;
    ASSERT_NEAR(rec.position.length(), 0.f, EPS);
    ASSERT_NEAR(dot(Vector3f(0.f, 0.f, 1.f), rec.normal), 1.f, EPS);
}

TEST(Triangle, bbox)
{
    std::array<Vector3f, 3> edges;
    for (auto& v : edges)
        v = randomVector3f();

    Triangle t(edges[0], edges[1], edges[2]);
    const auto bbox = t.bbox();

    for (const auto& v : edges)
    {
        EXPECT_LE(v, bbox.max());
        EXPECT_GE(v, bbox.min());
    }
}
