#pragma once

#include "Ray.h"
#include "scene/Parser.h"
#include "material/Material.h"
#include "HeapAligned.h"
#include "math/Transform.h"
#include <memory>

namespace gtrace {

class BBox;
class Shape : public SceneElement, public HeapAlignedType<Vector3f> {
public:
	Shape() = default;
	Shape(std::shared_ptr<Transform>& tr) : objectToWorld(tr) {}
	virtual ~Shape() {}

	/**
	 * Intersect a ray with the object.
	 * @param ray The ray
	 * @param tmin
	 * @param tmax The intersection point should be in the interval
	 * [ray.origin + tmin*ray.direction, ray.origin + tmax*ray.direction]
	 * tmix = 0, tmax = float::max if they are no restrains
	 * @param[out] rec Stores the information about the intersection point
	 * return true if the ray intersects the object, false otherwise
	 */
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const = 0;

	/**
     * Return a bounding box for the current object (idealy the smallest one)
	 */
	virtual BBox bbox() const = 0;

	static std::shared_ptr<Transform> parseTranformComponents(const SceneParser& parser, std::unordered_map<std::string, std::string>& map);
protected:
	void parseTransform(const SceneParser& parser, std::unordered_map<std::string, std::string>& map);
protected:
	std::shared_ptr<Transform> objectToWorld; /** Transform world coordiates to obj coordinates and vice versa */
};
}
