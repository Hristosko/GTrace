#include "pch.h"
#include "Utils.h"

#include "../geometry/BBox.cpp"
TEST(BBox, bound) {
	BBox a(Vector3f(0, 0, 0), Vector3f(1, 1, 1));
	BBox b(Vector3f(-1, -1, -1), Vector3f(0, 0, 0));
	BBox c = BBox::bound(a, b);
	EXPECT_TRUE(compare(c.min(), Vector3f(-1, -1, -1)));
	EXPECT_TRUE(compare(c.max(), Vector3f(1, 1, 1)));
}

TEST(BBox, intersect) {
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
}