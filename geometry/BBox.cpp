#include "BBox.h"

BBox BBox::bound(const BBox& a, const BBox& b) {
	return BBox(
		::min(a.bottom, b.bottom),
		::max(a.top, b.top)
	);
}
