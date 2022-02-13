#include "gtest/gtest.h"
#include "TestUtils.h"

#include "geometry/Sphere.h"
#include "geometry/BBox.h"
#include "parser/ParsedParams.h"

using namespace gtrace;

class SphereTest : public Test
{
public:
    static ParsedParams makeParams(float radius, const Vector3f position)
    {
        ParsedParams params;
        params.addFloat("radius", radius);
        params.addTranslation(position);
        return params;
    }

    static Sphere makeSphere(float radius, const Vector3f position = Vector3f(0.f))
    {
        return Sphere(makeParams(radius, position));
    }
};

TEST_F(SphereTest, intersect_axis)
{
    const Sphere s = makeSphere(1.f);
    const Ray ray(Vector3f(2.f, 0.f, 0.f), Vector3f(-1.f, 0.f, 0.f));
    Intersection intersection;
    ASSERT_TRUE(s.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(s.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    ASSERT_EQ(intersection.normal, Vector3f(1.f, 0.f, 0.f)); // outwards
    ASSERT_NEAR(intersection.time, 1.f, EPS);
}

TEST_F(SphereTest, outwards_ray)
{
    const Sphere s = makeSphere(1.f);
    const Ray ray(Vector3f(2.f, 0.f, 0.f), Vector3f(1.f, 0.f, 0.f));
    Intersection intersection;
    ASSERT_FALSE(s.hit(ray, 0.f, 1000.f, 0.f, &intersection));
}

TEST_F(SphereTest, random_ray_towards_the_center)
{
    const float radius = 2.f;
    const Sphere s = makeSphere(radius);
    const Vector3f origin = randomVector3f();
    const Vector3f target(0.f);
    const Vector3f direction = target - origin;
    const Ray ray(origin, normalize(direction));

    Intersection intersection;
    ASSERT_TRUE(s.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(s.hit(ray, 0.f, 1000.f, 0.f, &intersection));
    ASSERT_NEAR(intersection.normal.length(), 1.f, EPS);

    intersection.point = ray.origin + intersection.time * ray.direction;
    ASSERT_NEAR(intersection.point.length(), radius, EPS);
    const Vector3f normal = normalize(intersection.point);
    ASSERT_NEAR(dot(normal, intersection.normal), 1.f, EPS);
}

TEST_F(SphereTest, position)
{
    const Vector3f center(0.f, 0.f, 100.f);
    const Sphere s = makeSphere(1.f, center);
    Ray ray(Vector3f(2.f, 0.f, 0.f), Vector3f(-1.f, 0.f, 0.f));

    Intersection intersection;
    ASSERT_FALSE(s.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_FALSE(s.hit(ray, 0.f, 1000.f, 0.f, &intersection));

    ray.origin += center;
    ASSERT_TRUE(s.bbox().intersect(ray, 0.f, 1000.f));
    ASSERT_TRUE(s.hit(ray, 0.f, 1000.f, 0.f, &intersection));
}
