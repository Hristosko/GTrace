#include "gtest/gtest.h"
#include "Utils.h"

#include "geometry/Sphere.h"

using namespace gtrace;

TEST(Sphere, intersect_os)
{
    const Sphere s(1.f);
    const Ray ray(Vector3f(2.f, 0.f, 0.f), Vector3f(-1.f, 0.f, 0.f));
    HitRecord rec;
    ASSERT_TRUE(s.hit(ray, 0.f, 1000.f, 0.f, rec));
    ASSERT_EQ(rec.normal, Vector3f(1.f, 0.f, 0.f)); // outwards
    ASSERT_TRUE(compare(rec.t, 1.f));
}