#include "gtest/gtest.h"
#include "Utils.h"
#include "math/Utils.h"

using namespace gtrace;

TEST(Matrix4x4, mult) {
	{
		const float mf[] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
		const Matrix4x4 m(mf);
		const float expResf[] = { 90,100,110,120,202,228,254,280,314,356,398,440,426,484, 542, 600 };
		const Matrix4x4 expRes(expResf);
		const Matrix4x4 res = m * m;
		EXPECT_TRUE(compare(res, expRes));
	}
}

TEST(Matrix4x4, transposed) {
	const float mf[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 };
	const Matrix4x4 m(mf);
	const float expResf[] = { 0,4,8,12,1,5,9,13,2,6,10,14,3,7,11,15 };
	const Matrix4x4 expRes(expResf);
	const Matrix4x4 res = m.transposed();
	EXPECT_TRUE(compare(res, expRes));
}

TEST(Matrix4x4, mult2x2) {
	__m128 m = _mm_setr_ps(0, 1, 2, 3);
	__m128 d  = mult2x2(m, m);
	EXPECT_TRUE(compare(_mm_setr_ps(2,3,6,11), d, true));
}

TEST(Matrix4x4, multAdj2x2) {
	__m128 m = _mm_setr_ps(0, 1, 2, 3);
	__m128 d = multAdj2x2(m, m);
	EXPECT_TRUE(compare(_mm_setr_ps(-2,0,0,-2), d, true));
}

TEST(Matrix4x4, mult2xAdj2) {
	__m128 m = _mm_setr_ps(0, 1, 2, 3);
	__m128 d = mult2xAdj2(m, m);
	EXPECT_TRUE(compare(_mm_setr_ps(-2, 0, 0, -2), d, true));
}

TEST(Matrix4x4, inversed) {
	const float mf[] = { 1,0,0,1,0,2,1,2,2,1,0,1,2,0,1,4 };
	const Matrix4x4 m(mf);
	const float expResf[] = { -2,-0.5,1,0.5,
							1,0.5,0,-0.5,
							-8,-1,2,2,
							3,0.5,-1,-0.5 };
	const Matrix4x4 expRes(expResf);
	const Matrix4x4 res = m.inversed();
	EXPECT_TRUE(compare(res, expRes));
	const Matrix4x4 orig = res.inversed();
	EXPECT_TRUE(compare(m, orig));
}
