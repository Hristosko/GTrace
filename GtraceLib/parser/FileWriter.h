#pragma once
#include "File.h"

namespace gtrace
{
class FileWriter : public File
{
public:
    FileWriter(const char* path);

    void writeLine(const char* data);
};
}  // namespace gtrace