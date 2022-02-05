#include "gtest/gtest.h"
#include "Utils.h"
#include "math/Utils.h"

using namespace gtrace;

TEST(Transform, transform) {
	const Vector3f init(1, 2, 3);
	const float mf[] = { 1,0,0,1,0,2,1,2,2,1,0,1,2,0,1,4 };
	const Matrix4x4 m(mf);
	Transform tr(m);
	auto res = tr.transform(init);
	auto expRes = _mm_setr_ps(2.f/9, 1, 5.f/9, 1);
	EXPECT_TRUE(compare(expRes, res, true));

	res = tr.invTransform(init);
	expRes = _mm_setr_ps(1.f, 3.f, -4.f, 1.f);
	EXPECT_TRUE(compare(expRes, res, true));

	res = tr.transform(res);
	EXPECT_TRUE(compare(init, res, false));
}


TEST(Transform, translation) {
	const Vector3f init(1, 2, 3);
	const Vector3f trans(1, 1, 1);
	const Matrix4x4 m = Transform::makeTranslation(trans);
	Transform tr(m);
	const Vector3f res = tr.transform(init);
	EXPECT_TRUE(compare(init + trans, res));
}

TEST(Transform, rotation) {
	const Vector3f init(1, 1, 1);
	{
		const Matrix4x4 m = Transform::makeRotationX(180.f);
		Transform tr(m);
		const Vector3f rotX = tr.transform(init);
		EXPECT_TRUE(compare(Vector3f(1.f, -1.f, -1.f), rotX));
	}
	{
		const Matrix4x4 m = Transform::makeRotationY(180.f);
		Transform tr(m);
		const Vector3f rotY = tr.transform(init);
		EXPECT_TRUE(compare(Vector3f(-1.f, 1.f, -1.f), rotY));
	}
	{
		const Matrix4x4 m = Transform::makeRotationZ(180.f);
		Transform tr(m);
		const Vector3f rotZ = tr.transform(init);
		EXPECT_TRUE(compare(Vector3f(-1.f, -1.f, 1.f), rotZ));
	}
	{
		const Matrix4x4 m = Transform::makeRotationZ(0.f);
		Transform tr(m);
		const Vector3f rotZ = tr.transform(init);
		EXPECT_TRUE(compare(init, rotZ));
	}
}

TEST(Transform, scale) {
	const Vector3f init(1, 2, 3);
	const Matrix4x4 m = Transform::makeScale(2.f);
	Transform tr(m);
	Vector3f res = tr.transform(init);
	EXPECT_TRUE(compare(init * 2.f, res));
	res = tr.invTransform(init);
	EXPECT_TRUE(compare(init * 0.5f, res));
}

TEST(Transform, changeBasis) {
	const Vector3f init(1, 2, 3);
	const Matrix4x4 m = Transform::makeBasisChange(
		Vector3f(1.f, 1.f, 1.f),
		Vector3f(3.f, 2.f, 2.f),
		Vector3f(1.f, 1.f, -10.f)
	);
	Transform tr(m);
	const Vector3f res = tr.transform(init);
	const Vector3f expected(6.f, 13.f, -27.f);
	EXPECT_TRUE(compare(expected, res));
	const Vector3f reversed = tr.invTransformDirection(res);
	EXPECT_TRUE(compare(normalize(init), reversed));
}
