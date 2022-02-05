#include "gtest/gtest.h"
#include "TestUtils.h"

#include "geometry/Sphere.h"

using namespace gtrace;

TEST(Sphere, intersect_axis)
{
    const Sphere s(1.f);
    const Ray ray(Vector3f(2.f, 0.f, 0.f), Vector3f(-1.f, 0.f, 0.f));
    HitRecord rec;
    ASSERT_TRUE(s.hit(ray, 0.f, 1000.f, 0.f, rec));
    ASSERT_EQ(rec.normal, Vector3f(1.f, 0.f, 0.f)); // outwards
    ASSERT_NEAR(rec.t, 1.f, EPS);
}

TEST(Sphere, outwards_ray)
{
    const Sphere s(1.f);
    const Ray ray(Vector3f(2.f, 0.f, 0.f), Vector3f(1.f, 0.f, 0.f));
    HitRecord rec;
    ASSERT_FALSE(s.hit(ray, 0.f, 1000.f, 0.f, rec));
}

TEST(Sphere, random_ray_towards_the_center)
{
    const float radius = 2.f;
    const Sphere s(radius);
    const Vector3f origin = randomVector3f();
    const Vector3f target(0.f);
    const Vector3f direction = target - origin;
    const Ray ray(origin, normalize(direction));

    HitRecord rec;
    ASSERT_TRUE(s.hit(ray, 0.f, 1000.f, 0.f, rec));
    ASSERT_NEAR(rec.normal.length(), 1.f, EPS);

    rec.position = ray.origin + rec.t * ray.direction;
    ASSERT_NEAR(rec.position.length(), radius, EPS);
    const Vector3f normal = normalize(rec.position);
    ASSERT_NEAR(dot(normal, rec.normal), 1.f, EPS);
}
