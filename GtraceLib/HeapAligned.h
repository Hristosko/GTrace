#pragma once

namespace gtrace {

/**
 * Objects that require extra aligment (Vector3f) should inherit this class.
 * This is an issue only on Windows, not on Linux.
 */
template<size_t N>
class HeapAligned {
public:
#ifdef _WIN32
	void* operator new(size_t size) {
		return ::operator new(size, std::align_val_t(N));
	}
	void operator delete(void* ptr) {
		::operator delete(ptr, std::align_val_t(N));
	}
#endif
};
}
