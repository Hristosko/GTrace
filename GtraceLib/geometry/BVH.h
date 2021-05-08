#pragma once

#include "Shape.h"
#include "BBox.h"

class BVH : public Shape {
public:
	BVH(Shape* l, Shape* r)
		: left(l),
		right(r),
		box(BBox::bound(l->bbox(), r->bbox())) {}
	BVH(Shape* l, Shape* r, const BBox& box)
		: left(l),
		right(r),
		box(box) {}

	static Shape* build(Shape** shapes, uint32_t cnt);

	virtual ~BVH() override;
	// not used, cannot be instanciated from the parser
	virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override {}
	virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
	virtual BBox bbox() const override { return this->box; }
private:
	Shape* left;
	Shape* right;
	BBox box;
};