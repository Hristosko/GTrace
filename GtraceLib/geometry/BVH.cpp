#include "BVH.h"
#include "common/Logger.h"

namespace gtrace
{
static void swapWithMove(std::unique_ptr<Shape>& a, std::unique_ptr<Shape>& b)
{
    std::unique_ptr<Shape> c = std::move(a);
    a = std::move(b);
    b = std::move(c);
}

/**
 * Split the shapes in two using a pivot element.
 * The splitting is done alongside a given axis.
 * @param[in, out] shapes The shapes to be splitted.
 * @param cnt The number of shapes
 * @param pivot The pivot element
 * @param axis The axis along which the splitting should be done
 * (0 for x, 1 for y and 2 for z)
 * @return The number of elemnts in the left part of the split (lesser elemnts)
 */
static size_t split(std::unique_ptr<Shape>* shapes, size_t cnt, float pivot, int axis)
{
    size_t lesser = 0;
    for (size_t i = 0; i < cnt; ++i)
    {
        const BBox bb = shapes[i]->bound();
        const float center = (bb.max() + bb.min())[axis] * 0.5f;
        if (center < pivot)
        {
            swapWithMove(shapes[i], shapes[lesser++]);
        }
    }
    if (lesser == 0 || lesser == cnt)
        lesser = (cnt / 2);
    return lesser;
}

/**
 * Build a KD Tree recursively.
 * @param shapes All shapes that will be part of the tree.
 * @param cnt The number of shapes
 * @param axis The current axis
 */
static std::unique_ptr<Shape> build(std::unique_ptr<Shape>* shapes, size_t cnt, int axis)
{
    if (cnt == 1)
    {
        std::unique_ptr<Shape> res = std::move(shapes[0]);
        return res;
    }
    if (cnt == 2)
        return std::make_unique<BVH>(std::move(shapes[0]), std::move(shapes[1]));

    // find the midpoint of the bounding box according the given axis
    BBox box = shapes[0]->bound();
    for (uint32_t i = 0; i < cnt; ++i)
        box = BBox::bound(box, shapes[i]->bound());

    const Vector3f pivot = (box.max() - box.min()) * 0.5f;
    size_t middle = split(shapes, cnt, pivot[axis], axis);

    return std::make_unique<BVH>(
        build(shapes, middle, (axis + 1) % 3), build(&shapes[middle], cnt - middle, (axis + 1) % 3), box);
}

/**
 * Build a KD Tree, see Shape* build(Shape** shapes, uint32_t cnt, int axis)
 */
std::unique_ptr<Shape> BVH::build(std::unique_ptr<Shape>* shapes, size_t cnt)
{
    LOGINFO("Start building bounding volume hierarchie. Shapes count: ", cnt);
    std::unique_ptr<Shape> res = gtrace::build(shapes, cnt, 0);
    LOGINFO("Finish building bounding volume hierarchie.");
    return res;
}

bool BVH::hit(const Ray& ray, float tmin, float tmax, float time, Intersection* intersection) const
{
    if (!this->box.intersect(ray, tmin, tmax))
        return false;

    intersection->time = tmax;  // the left hit might upate that
    // we have to call both branches since we can have 2 hits
    bool ishit = this->left->hit(ray, tmin, tmax, time, intersection);
    bool ishit1 = this->right->hit(ray, tmin, intersection->time, time, intersection);

    return ishit || ishit1;
}
}  // namespace gtrace
