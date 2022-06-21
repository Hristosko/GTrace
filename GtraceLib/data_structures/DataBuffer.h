#pragma once
#include <assert.h>
#include <memory>
#include <stdint.h>

namespace gtrace
{
class DataBuffer
{
public:
    DataBuffer() = default;
    DataBuffer(const DataBuffer& rhs);
    DataBuffer(DataBuffer&&) = default;
    DataBuffer& operator=(const DataBuffer& rhs);
    DataBuffer& operator=(DataBuffer&&) = default;

    template<typename T>
    void init(uint64_t bufSize)
    {
        init(sizeof(T), bufSize);
    }
    void init(uint64_t tSize, uint64_t bufSize);

    template<typename T>
    T* getAt(uint64_t idx) const
    {
        return reinterpret_cast<T*>(getAt(idx));
    }
    void* getAt(uint64_t idx) const;

    template<typename T>
    T& at(uint64_t idx)
    {
        return *getAt<T>(idx);
    }

    template<typename T>
    const T& at(uint64_t idx) const
    {
        return *getAt<T>(idx);
    }

    void putAt(uint64_t idx, const void* data, uint64_t dataSize);

    bool isInitialized() const;
    uint64_t getElementSize() const { return typeSize; }
    uint64_t getSize() const { return size; }

    bool operator==(const DataBuffer& rhs) const;
    bool operator!=(const DataBuffer& rhs) const { return !(*this == rhs); }

private:
    uint64_t typeSize;
    uint64_t size;
    std::unique_ptr<uint8_t[]> buffer;
};

}  // namespace gtrace