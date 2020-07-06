#pragma once

#include "Shape.h"

__declspec(align(16)) class Triangle : public Shape {
public:
	Triangle() = default;

	void parse(std::unordered_map<std::string, std::string>& map) override;
	bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;

	void* operator new(size_t s);
	void operator delete(void* p);
private:
	Vector3f a, b, c;
	Material* mat;
};