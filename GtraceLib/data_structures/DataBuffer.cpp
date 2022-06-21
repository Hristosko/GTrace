#include <assert.h>
#include <cstring>
#include "DataBuffer.h"

namespace gtrace
{
DataBuffer::DataBuffer(const DataBuffer& rhs)
{
    *this = rhs;
}

DataBuffer& DataBuffer::operator=(const DataBuffer& rhs)
{
    init(rhs.typeSize, rhs.size);
    memcpy(getAt(0), rhs.getAt(0), size * typeSize);
    return *this;
}

void DataBuffer::init(uint64_t tSize, uint64_t bufSize)
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

bool DataBuffer::operator==(const DataBuffer& rhs) const
{
    if (typeSize != rhs.typeSize || size != rhs.size)
        return false;

    return memcmp(buffer.get(), rhs.buffer.get(), typeSize * size) == 0;
}

}  // namespace gtrace