#pragma once

#include <memory>
#include <cstdint>

class DataBuffer {
public:
	DataBuffer() : typeSize(0), bufferSize(0), buffer(nullptr) {}
	DataBuffer(uint16_t typeS, uint64_t bufferS)
		: typeSize(typeS), bufferSize(bufferS), buffer(new char[bufferS * typeS]) {}

	void* getBuffer() { return this->buffer.get(); }
	void* ptrByIdx(uint64_t idx) {
		if (idx >= this->bufferSize || !this->buffer) return nullptr;
		char* res = reinterpret_cast<char*>(this->buffer.get());
		res += (this->typeSize * idx);
		return res;
	}

	void init(uint16_t typeS, uint64_t bufferS) {
		this->typeSize = typeS;
		this->bufferSize = bufferS;
		this->buffer = std::unique_ptr<char>(new char[bufferS * typeS]);
	}

	template<typename T>
	void init(uint64_t bufferS) {
		this->init(sizeof(T), bufferS);
	}
private:
	uint16_t typeSize;
	uint64_t bufferSize;
	std::unique_ptr<char> buffer;
};