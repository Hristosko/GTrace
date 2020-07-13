#include "pch.h"
#include "Utils.h"

TEST(Vector3f, maxElement) {
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

TEST(Vector3f, minElement) {
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

TEST(Vector3f, comparing) {
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
		EXPECT_FALSE(b <= a);
	}
}