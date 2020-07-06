#pragma once

#include "Shape.h"

__declspec(align(16)) class Sphere : public Shape {
public:
	void parse(std::unordered_map<std::string, std::string>& map) override;
	bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecort& rec) const override;

	void* operator new(size_t s);
	void operator delete(void* p);
private:
	Vector3f center;
	float radius;
	Texture* text;
};
