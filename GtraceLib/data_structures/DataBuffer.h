#pragma once
#include <memory>
#include <stdint.h>

namespace gtrace
{
class DataBuffer
{
public:
    template<typename T>
    void init(uint64_t bufSize)
    {
        init(sizeof(T), bufSize);
    }
    void init(uint16_t tSize, uint64_t bufSize);

    template<typename T>
    T* getAt(uint64_t idx) const
    {
        assert(sizeof(T) == typeSize);
        return reinterpret_cast<T*>(getAt(idx));
    }
    void* getAt(uint64_t idx) const;

    void putAt(uint64_t idx, const void* data, uint64_t dataSize);

    bool isInitialized() const;

private:
    uint16_t typeSize;
    uint64_t size;
    std::unique_ptr<uint8_t[]> buffer;
};

}  // namespace gtrace