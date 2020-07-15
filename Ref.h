#pragma once

#include <atomic>

///
/// Base class that implements the interface required by Ref.
/// Used for ref. count of objects
///
class ReferenceCounted {
public:
	ReferenceCounted() : refCount(0) {}
	void addRef() {
		this->refCount.fetch_add(1, std::memory_order_relaxed);
	}
	// return true if the result is 0
	bool removeRef() {
		return this->refCount.fetch_sub(1, std::memory_order_acq_rel) == 0;
	}
protected:
	std::atomic_int refCount;
};

///
/// Intrusive pointer impl that uses ReferenceCounted interface
///
template<typename T>
class Ref {
public:
	Ref() : ptr(nullptr) {}
	Ref(T* p) : ptr(p) {}
	~Ref() {
		this->free();
	}

	Ref(const Ref& rhs) : ptr(rhs.ptr) {
		if (ptr) ptr->addRef();
	}
	Ref(Ref&& rhs) : ptr(rhs.ptr) {
		rhs.ptr = nullptr;
	}

	Ref& operator=(const Ref& rhs) {
		if (this != rhs) {
			this->free();
			this->ptr = rhs.ptr;
			if (this->ptr) this->ptr->addRef();
		}
	}

	Ref&& operator=(Ref&& rhs) {
		if (this != &rhs) {
			this->free();
			this->ptr = rhs.ptr;
			rhs.ptr = nullptr;
		}
	}

	T* operator->() { return this->ptr; }
	T* get() { return this->ptr(); }

	void free() {
		if (ptr && ptr->removeRef()) delete ptr;
	}
private:
	T* ptr;
};