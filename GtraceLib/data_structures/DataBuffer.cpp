#include <assert.h>
#include <cstring>
#include "DataBuffer.h"

namespace gtrace
{
void DataBuffer::init(uint16_t tSize, uint64_t bufSize)
{
    typeSize = tSize;
    size = bufSize;
    buffer = std::make_unique<uint8_t[]>(typeSize * size);
}

bool DataBuffer::isInitialized() const
{
    return typeSize != 0;
}

void* DataBuffer::getAt(uint64_t idx) const
{
    assert(idx < size);
    return buffer.get() + idx * typeSize;
}

void DataBuffer::putAt(uint64_t idx, const void* data, uint64_t dataSize)
{
    assert(idx + dataSize <= size);
    void* dest = getAt(idx);
    memcpy(dest, data, dataSize * typeSize);
}

}  // namespace gtrace