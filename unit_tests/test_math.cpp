#include "gtest/gtest.h"
#include "Utils.h"
#include "math/Utils.h"

using namespace gtrace;

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

TEST(Vector3f, sqrt) {
	const Vector3f v(0.25f, 10.f, 625.f);
	const Vector3f expected(sqrtf(0.25f), sqrtf(10.f), sqrtf(625.f));
	const Vector3f res = sqrt(v);
	EXPECT_TRUE(compare(expected, res));
}

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

TEST(SphericalCoordinates, Theta) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 60)
	Vector3f vec(0.25f, 0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	float theta = degToRad(30);
	ASSERT_TRUE(compare(cosf(theta), cosTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta)*sinf(theta), sin2Theta(vec)));

	// The spherical coordinates are: (r=1, theta = 30, phi = 120)
	vec = normalize(Vector3f(-0.25f, 0.4330127019f, 0.8660254038f));
	theta = degToRad(30);
	ASSERT_TRUE(compare(cosf(theta), cosTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta) * sinf(theta), sin2Theta(vec)));
}

TEST(SphericalCoordinates, Theta2ndQuad) {
	// The spherical coordinates are: (r=1, theta = 150, phi = 60)
	Vector3f vec(0.25f, 0.4330127019f, -0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(150);
	ASSERT_TRUE(compare(cosf(theta), cosTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinTheta(vec)));
	ASSERT_TRUE(compare(sinf(theta) * sinf(theta), sin2Theta(vec)));
}

TEST(SphericalCoordinates, Phi) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 60)
	Vector3f vec(0.25f, 0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(60);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}

TEST(SphericalCoordinates, Phi2ndQuad) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 120)
	Vector3f vec(-0.25f, 0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(120);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}

TEST(SphericalCoordinates, Phi3rdQuad) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 240)
	Vector3f vec(-0.25f, -0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(240);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}

TEST(SphericalCoordinates, Phi4rdQuad) {
	// The spherical coordinates are: (r=1, theta = 30, phi = 300)
	Vector3f vec(0.25f, -0.4330127019f, 0.8660254038f);
	vec = normalize(vec);

	const float theta = degToRad(300);
	ASSERT_TRUE(compare(cosf(theta), cosPhi(vec)));
	ASSERT_TRUE(compare(sinf(theta), sinPhi(vec)));
}
