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

TEST(Vector3f, isAtLeastOneGreaterThan) {
	{
		const Vector3f v(0.f, 2.f, 0.f);
		bool res = v.isAtLeastOneGreaterThan(1.f);
		EXPECT_TRUE(res);
	}
}

TEST(Matrix4x4, mult) {
	{
		const float mf[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		const Matrix4x4 m(mf);
		const float expResf[] = { 90,100,110,120,202,225,254,280,314,356,398,440,426,484, 542, 600 };
		const Matrix4x4 expRes(expResf);
		const Matrix4x4 res = m * m;
		EXPECT_TRUE(compare(res, expRes));
	}
}