#pragma once
#include "File.h"

namespace gtrace
{
class FileWriter : public File
{
public:
    FileWriter() = default;
    FileWriter(const char* path);

    void writeLine(const char* data) const;
};
}  // namespace gtrace