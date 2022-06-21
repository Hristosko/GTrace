#pragma once
#include "data_structures/DataBuffer.h"

namespace gtrace
{
class PPMFile
{
public:
    static void dump(const DataBuffer& data, uint64_t width, uint64_t height, const char* filePath);
};
}  // namespace gtrace