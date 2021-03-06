#pragma once

#include "Shape.h"

namespace gtrace {

/**
 * Represents a triangle. Mostrly usefull for meshes as utility fiunctions.
 * This class enables using the triangle as a standalone object.
 */
class Triangle : public Shape {
public:
	static bool hit(const Vector3f& a, const Vector3f& b, const Vector3f& c,const Transform* tr,
		const Ray& ray, float tmin, float tmax,
		float& beta, float& gamma, float& tval);

	static BBox triangleBBox(const Vector3f& a, const Vector3f& b, const Vector3f& c, const Transform* tr);

	Triangle() = default;
	Triangle(const Vector3f& a, const Vector3f& b, const Vector3f& c)
		: a(a), b(b), c(c), mat(nullptr) {}

	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override;
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
	virtual BBox bbox() const override;

private:
	Vector3f a, b, c; /** The tree vertices of the triangle */
	Material* mat; /** The material of the triangle */
};
}
