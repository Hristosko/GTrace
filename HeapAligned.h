#pragma once

/**
 * Objects that require extra aligment (Vector3f) should inherit this class.
 */
template<size_t N>
class HeapAligned {
public:
	void* operator new(size_t size) {
		return ::operator new(size, std::align_val_t(N));
	}
	void operator delete(void* ptr) {
		::operator delete(ptr, std::align_val_t(N));
	}
};