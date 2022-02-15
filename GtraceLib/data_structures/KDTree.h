#pragma once
#include <memory>
#include "geometry/Bounded.h"

namespace gtrace
{
template<typename BoundingVolume, int Dimensions>
class KDTree
{
public:
    using BoundedType = Bounded<BoundingVolume>;
    struct Node : BoundedType
    {
        BoundingVolume bounding;
        std::unique_ptr<BoundedType> left;
        std::unique_ptr<BoundedType> right;

        BoundingVolume bound() const override { return bounding; }

        Node(std::unique_ptr<BoundedType>&& left, std::unique_ptr<BoundedType>&& right) :
            bounding(BoundingVolume::bound(left->bound(), right->bound())),
            left(move(left)),
            right(move(right))
        {
        }
    };

    bool isEmpty() const { return root == nullptr; }

protected:
    std::unique_ptr<BoundedType> root;
};
}  // namespace gtrace
