#pragma once

#include <memory>
#include <cstdint>

namespace gtrace {

class DataBuffer {
public:
	DataBuffer() : typeSize(0), bufferSize(0), buffer(nullptr) {}
	DataBuffer(uint16_t typeS, uint64_t bufferS)
		: typeSize(typeS), bufferSize(bufferS),
		buffer(std::make_unique<char[]>(std::size_t(bufferS * typeS))) {}

	void* getBuffer() { return this->buffer.get(); }
	void* ptrByIdx(uint64_t idx) {
		if (idx >= this->bufferSize || !this->buffer) return nullptr;
		char* res = reinterpret_cast<char*>(this->buffer.get());
		res += (this->typeSize * idx);
		return res;
	}

	const void* ptrByIdx(uint64_t idx) const {
		if (idx >= this->bufferSize || !this->buffer) return nullptr;
		char* res = reinterpret_cast<char*>(this->buffer.get());
		res += (this->typeSize * idx);
		return res;
	}

	template<typename T>
	void copyValue(uint64_t pos, const DataBuffer& src, uint64_t srcPos) {
		{
			T* dest = reinterpret_cast<T*>(this->ptrByIdx(pos));
			const T* src = reinterpret_cast<const T*>(src.ptrByIdx(srcPos));
			*dest = *src;
		}
	}

	void init(uint16_t typeS, uint64_t bufferS) {
		this->typeSize = typeS;
		this->bufferSize = bufferS;
		this->buffer = std::make_unique<char[]>(std::size_t(bufferS * typeS));
	}

	template<typename T>
	void init(uint64_t bufferS) {
		this->init(sizeof(T), bufferS);
	}
private:
	uint16_t typeSize;
	uint64_t bufferSize;
	std::unique_ptr<char[]> buffer;
};
}
