#include "gtest/gtest.h"
#include "TestUtils.h"

#include "geometry/BBox.h"

using namespace gtrace;
TEST(BBox, bound)
{
    BBox a(Vector3f(0.f), Vector3f(1.f));
    BBox b(Vector3f(-1.f), Vector3f(0.f));
    BBox c = BBox::bound(a, b);
    ASSERT_EQ(c.min(), Vector3f(-1.f));
    ASSERT_EQ(c.max(), Vector3f(1.f));
}

TEST(BBox, intersect)
{
    BBox box(Vector3f(0.f), Vector3f(1.f));
    {
        Ray ray(Vector3f(-0.5f, 0.5f, 0.5f), Vector3f(1.f, 0.f, 0.f));
        EXPECT_TRUE(box.intersect(ray, 0.f, 1.f));
    }
    {
        Ray ray(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.f, 1.f, 0.f));
        EXPECT_TRUE(box.intersect(ray, 0.f, 1.f));
    }
    {
        Ray ray(Vector3f(0.5f, -0.5f, 0.5f), Vector3f(0.f, -1.f, 0.f));
        EXPECT_FALSE(box.intersect(ray, 0.f, 1.f));
    }
    box = BBox(Vector3f(0.f), Vector3f(1.f, 1.f, 0.f));
    {
        Ray ray(Vector3f(0.5f, 0.5f, 0.5f), Vector3f(0.f, 0.f, -1.f));
        EXPECT_TRUE(box.intersect(ray, 0.f, 1.f));
    }
}

TEST(BBox, nan_inf_rayInvDirection)
{
    BBox box(Vector3f(0.f), Vector3f(1.f));
    Ray ray(Vector3f(0.f, 0.f, -0.5f), Vector3f(0.f, 0.f, 1.f));
    EXPECT_TRUE(box.intersect(ray, 0.f, 1.f));
}
