#include "gtest/gtest.h"
#include "TestUtils.h"
#include "math/Utils.h"

using namespace gtrace;

TEST(Vector3f, maxElement)
{
    {
        const Vector3f v(1, 2, 3);
        const float f = 4;
        const float res = maxElement(v, f);
        EXPECT_TRUE(compare(res, 4));
    }
    {
        const Vector3f v(4, 2, 3);
        const float f = 1;
        const float res = maxElement(v, f);
        EXPECT_TRUE(compare(res, 4));
    }
}

TEST(Vector3f, minElement)
{
    {
        const Vector3f v(1, 2, 3);
        const float f = -1;
        const float res = minElement(v, f);
        EXPECT_TRUE(compare(res, -1));
    }
    {
        const Vector3f v(-1, 2, 3);
        const float f = 1;
        const float res = minElement(v, f);
        EXPECT_TRUE(compare(res, -1));
    }
}

TEST(Vector3f, comparing)
{
    {
        const Vector3f a(0.f);
        const Vector3f b(1.f);
        EXPECT_TRUE(a <= b);
        EXPECT_FALSE(b <= a);

        EXPECT_TRUE(a < b);
        EXPECT_FALSE(b < a);
    }
    {
        const Vector3f a(0.f);
        const Vector3f b(-1.f, 2.f, -2.f);
        EXPECT_FALSE(b <= a);
        EXPECT_FALSE(a <= b);
    }
}

TEST(Vector3f, isAtLeastOneGreaterThan)
{
    {
        const Vector3f v(0.f, 2.f, 0.f);
        bool res = v.isAtLeastOneGreaterThan(1.f);
        EXPECT_TRUE(res);
    }
}

TEST(Vector3f, sqrt)
{
    const Vector3f v(0.25f, 10.f, 625.f);
    const Vector3f expected(sqrtf(0.25f), sqrtf(10.f), sqrtf(625.f));
    const Vector3f res = sqrt(v);
    EXPECT_TRUE(compare(expected, res));
}
