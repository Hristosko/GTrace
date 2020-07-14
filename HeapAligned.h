#pragma once

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