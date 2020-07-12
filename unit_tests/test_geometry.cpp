#include "pch.h"

#include "../geometry/BBox.cpp"

#define EPS 0.001

static bool compare(float a, float b) {
	return fabsf(a - b) < EPS;
}

static bool compare(const Vector3f& a, const Vector3f& b) {
	return compare(a.x(), b.x()) ||
		compare(a.y(), b.y()) ||
		compare(a.z(), b.z());
}

TEST(BBox, bound) {
	BBox a(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
	BBox b(Vector3f(-1, -1, -1), Vector3f(0, 0, 0));
	BBox c = BBox::bound(a, b);
	EXPECT_TRUE(compare(c.min(), Vector3f(-1, -1, -1)));
	EXPECT_TRUE(compare(c.max(), Vector3f(1, 1, 1)));
}