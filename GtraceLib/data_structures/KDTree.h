#pragma once
#include <algorithm>
#include <assert.h>
#include <type_traits>
#include <vector>
#include "common/Common.h"
#include "geometry/Bounded.h"

namespace gtrace
{
template<typename BoundingVolume, typename Base>
struct KDTreeNode : Base
{
    using BoundedPtr = std::unique_ptr<Bounded<BoundingVolume>>;
    using BasePtr = std::unique_ptr<Base>;
    using BaseType = Base;

    BoundingVolume bounding;
    BasePtr left;
    BasePtr right;

    BoundingVolume bound() const override { return bounding; }

    KDTreeNode(BoundedPtr&& left, BoundedPtr&& right) :
        bounding(BoundingVolume::bound(left->bound(), right->bound())),
        left(staticCastPointer<Base>(move(left))),
        right(staticCastPointer<Base>(move(right)))
    {
        static_assert(std::is_base_of_v<Bounded<BoundingVolume>, Base>, "KDTRee node must inherit Bounded");
    }
};

template<typename BoundingVolume>
struct KDTreeDefaultNode : KDTreeNode<BoundingVolume, Bounded<BoundingVolume>>
{
    using BoundedType = Bounded<BoundingVolume>;
    using BoundedPtr = std::unique_ptr<BoundedType>;

    KDTreeDefaultNode(BoundedPtr&& left, BoundedPtr&& right) :
        KDTreeNode<BoundingVolume, Bounded<BoundingVolume>>(move(left), move(right))
    {
    }
};

template<typename BoundingVolume, typename Node = KDTreeDefaultNode<BoundingVolume>, int Dimensions = 3>
class KDTree
{
public:
    using BoundedType = Bounded<BoundingVolume>;
    using BoundedPtr = std::unique_ptr<BoundedType>;
    using BuildBuffer = std::vector<BoundedPtr>;
    struct Comparator
    {
        explicit Comparator(int axis) : axis(axis) {}
        int axis;
        bool operator()(const BoundedPtr& lhs, const BoundedPtr& rhs)
        {
            return lhs->bound().center(axis) < rhs->bound().center(axis);
        }
    };

    bool isEmpty() const { return root == nullptr && buildBuffer.empty(); }
    bool isBuilt() const { return root != nullptr; }

    void clear()
    {
        root.reset();
        buildBuffer.clear();
    }

    void insert(BoundedPtr&& element)
    {
        assert(!isBuilt());
        buildBuffer.emplace_back(move(element));
    }

    void build()
    {
        assert(!isBuilt());
        root = build(0, buildBuffer.size(), 0);
        buildBuffer.clear();
    }

    const typename Node::BaseType* data() const
    {
        assert(isBuilt());
        return static_cast<typename Node::BaseType*>(root.get());
    }

private:
    BoundedPtr build(size_t from, size_t to, int axis)
    {
        const auto count = to - from;
        assert(count != 0);
        if (count == 1)
            return std::move(buildBuffer[from]);
        if (count == 2)
            return std::make_unique<Node>(std::move(buildBuffer[from]), std::move(buildBuffer[from + 1]));

        std::sort(buildBuffer.begin() + from, buildBuffer.begin() + to, Comparator(axis));
        const auto mid = (to + from) / 2;
        const auto nextAxis = (axis + 1) % Dimensions;
        return std::make_unique<Node>(build(from, mid, nextAxis), build(mid, to, nextAxis));
    }

protected:
    BoundedPtr root;
    BuildBuffer buildBuffer;
};
}  // namespace gtrace
