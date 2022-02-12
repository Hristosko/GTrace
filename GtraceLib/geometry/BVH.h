#pragma once

#include "Shape.h"
#include "BBox.h"

namespace gtrace {

/**
 * Bounding volume hierarchie. Used to accelerate the  intersection of the rays
 * with the geometry. Uses a KD Tree.
 */
class BVH : public Shape {
public:
    BVH(std::unique_ptr<Shape>&& l, std::unique_ptr<Shape>&& r)
        : left(std::move(l)),
        right(std::move(r)),
        box(BBox::bound(left->bbox(), right->bbox())) {}
    BVH(std::unique_ptr<Shape>&& l, std::unique_ptr<Shape>&& r, const BBox& box)
        : left(std::move(l)),
        right(std::move(r)),
        box(box) {}

    static std::unique_ptr<Shape> build(std::unique_ptr<Shape>* shapes, size_t cnt);

    // not used, cannot be instanciated from the parser
    virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) override {}
    virtual bool hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const override;
    virtual BBox bbox() const override { return this->box; }
private:
    std::unique_ptr<Shape> left; /** The left subtree */
    std::unique_ptr<Shape> right; /** The right subtree */
    BBox box; /** The bbox of the current node */
};
}
