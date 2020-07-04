#pragma once

#include "Shape.h"
#include "../scene/Parser.h"

__declspec(align(16)) class Triangle : public Shape, public SceneElement {
public:
	Triangle() = default;
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Vector3f& color)
		: a(a), b(b), c(c), color(color) {}

	void parse(std::unordered_map<std::string, std::string>& map) override;
	bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const override;

	void* operator new(size_t s);
	void operator delete(void* p);
private:
	Vector3f a, b, c;
	Vector3f color;
};