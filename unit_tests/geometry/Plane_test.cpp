#include <array>
#include "gtest/gtest.h"
#include "TestUtils.h"

#include "geometry/Plane.h"
#include "geometry/BBox.h"

using namespace gtrace;

TEST(Plane, intersect_axis)
{
    const float size = 1.f;
    Plane p(size, size);
    const Ray ray(Vector3f(0.f, 0.0f, 2.f), Vector3f(0.f, 0.f, -1.f));
    HitRecord rec;
    ASSERT_TRUE(p.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(p.hit(ray, 0.f, 1000.f, 0.f, rec));
}

TEST(Plane, outwards_ray)
{
    const float size = 1.f;
    Plane p(size, size);
    const Ray ray(Vector3f(0.f, 0.0f, 2.f), Vector3f(0.f, 0.f, 1.f));
    HitRecord rec;
    ASSERT_FALSE(p.hit(ray, 0.f, 1000.f, 0.f, rec));
}

TEST(Plane, random_ray_towards_the_center)
{
    const float size = 1.f;
    Plane p(size, size);
    const Vector3f origin = randomVector3f();
    const Vector3f target(0.f);
    const Vector3f direction = target - origin;
    const Ray ray(origin, normalize(direction));

    HitRecord rec;
    ASSERT_TRUE(p.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(p.hit(ray, 0.f, 1000.f, 0.f, rec));
    ASSERT_NEAR(rec.normal.length(), 1.f, EPS);

    rec.position = ray.origin + rec.t * ray.direction;
    ASSERT_NEAR(rec.position.length(), 0.f, EPS);
    ASSERT_NEAR(dot(Vector3f(0.f, 0.f, 1.f), rec.normal), 1.f, EPS);
}
