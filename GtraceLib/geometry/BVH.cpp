#include "BVH.h"
#include "Logger.h"

namespace gtrace {

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
static size_t split(Shape** shapes, size_t cnt, float pivot, int axis) {
	size_t lesser = 0;
	for (size_t i = 0; i < cnt; ++i) {
		const BBox bb = shapes[i]->bbox();
		const float center = (bb.max() + bb.min())[axis] * 0.5f;
		if (center < pivot) {
			std::swap(shapes[i], shapes[lesser++]);
		}
	}
	if (lesser == 0 || lesser == cnt) lesser = (cnt / 2);
	return lesser;
}

/**
 * Build a KD Tree recursively.
 * @param shapes All shapes that will be part of the tree.
 * @param cnt The number of shapes
 * @param axis The current axis
 */
static Shape* build(Shape** shapes, size_t cnt, int axis) {
	if (cnt == 1) {
		Shape* res = shapes[0];
		// transfer ownership
		shapes[0] = nullptr;
		return res;
	}
	if (cnt == 2) {
		BVH* res = new BVH(shapes[0], shapes[1]);
		shapes[0] = nullptr;
		shapes[1] = nullptr;
		return res;
	}

	// find the midpoint of the bounding box according the given axis
	BBox box = shapes[0]->bbox();
	for (uint32_t i = 0; i < cnt; ++i) {
		box = BBox::bound(box, shapes[i]->bbox());
	}
	const Vector3f pivot = (box.max() - box.min()) * 0.5f;
	size_t middle = split(shapes, cnt, pivot[axis], axis);

	return new BVH(
		build(shapes, middle, (axis + 1) % 3),
		build(&shapes[middle], cnt - middle, (axis + 1) % 3),
		box);
}

/**
 * Build a KD Tree, see Shape* build(Shape** shapes, uint32_t cnt, int axis)
 */
Shape* BVH::build(Shape** shapes, size_t cnt) {
	LOGINFO("Start building bounding volume hierarchie. Shapes count: ", cnt);
	Shape* res =  gtrace::build(shapes, cnt, 0);
	LOGINFO("Finish building bounding volume hierarchie.");
	return res;
}

BVH::~BVH() {
	delete this->left;
	delete this->right;
}

bool BVH::hit(const Ray& ray, float tmin, float tmax, float time, HitRecord& rec) const {
	if (!this->box.intersect(ray, tmin, tmax)) return false;

	rec.t = tmax; // the left hit might upate that
	// we have to call both branches since we can have 2 hits
	bool ishit = this->left->hit(ray, tmin, tmax, time, rec);
	bool ishit1 = this->right->hit(ray, tmin, rec.t, time, rec);

	return ishit || ishit1;
}
}
