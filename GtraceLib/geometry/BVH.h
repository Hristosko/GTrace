#pragma once

#include "Shape.h"
#include "BBox.h"
#include "data_structures/KDTree.h"
namespace gtrace
{
/**
 * Bounding volume hierarchie. Used to accelerate the  intersection of the rays
 * with the geometry. Uses a KD Tree.
 */
class BVH
{
public:
    struct Node : KDTreeNode<BBox, Shape>
    {
        using InternalNode = KDTreeNode<BBox, Shape>;
        Node(BoundedPtr&& left, BoundedPtr&& right) : InternalNode(move(left), move(right)) {}
        bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* intersection) const final;
    };

    bool hit(const Ray& ray, float tmin, float tmax, float time, Intersection* intersection) const;

    void build();
    void insert(std::unique_ptr<Shape>&& shape);

private:
    KDTree<BBox, Node> tree;
};
}  // namespace gtrace
