#pragma once

#include "Ray.h"
#include "../scene/Parser.h"
#include "../material/Material.h"
#include "../HeapAligned.h"
#include "../math/Transform.h"

class BBox;
class Shape : public SceneElement, public HeapAligned<16> {
public:
	virtual ~Shape() {}

	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const = 0;
	virtual BBox bbox() const = 0;
protected:
	void parseTransform(std::unordered_map<std::string, std::string>& map);
protected:
	Ref<Transform> objectToWorld;
};