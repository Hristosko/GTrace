#include "BVH.h"
#include "../Logger.h"

///
/// Quicksort-style split function to split the shapes according to the pivot value and axis
///
static uint32_t split(Shape** shapes, uint32_t cnt, float pivot, int axis) {
	uint32_t lesser = 0;
	for (uint32_t i = 0; i < cnt; ++i) {
		const BBox bb = shapes[i]->bbox();
		const float center = (bb.max() + bb.min())[axis] * 0.5f;
		if (center < pivot) {
			std::swap(shapes[i], shapes[lesser++]);
		}
	}
	if (lesser == 0 || lesser == cnt) lesser = (cnt / 2);
	return lesser;
}

static Shape* build(Shape** shapes, uint32_t cnt, int axis) {
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
	uint32_t middle = split(shapes, cnt, pivot[axis], axis);

	return new BVH(
		build(shapes, middle, (axis + 1) % 3),
		build(&shapes[middle], cnt - middle, (axis + 1) % 3),
		box);
}

Shape* BVH::build(Shape** shapes, uint32_t cnt) {
	LOGINFO("Start building bounding volume hierarchie.");
	Shape* res =  ::build(shapes, cnt, 0);
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
