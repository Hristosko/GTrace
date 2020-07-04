#pragma once

#include "Shape.h"

class Triangle : public Shape {
public:
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Vector3f& color)
		: a(a), b(b), c(c), color(color) {}

	bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const override;
private:
	Vector3f a, b, c;
	Vector3f color;
};