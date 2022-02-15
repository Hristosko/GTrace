#pragma once
#include <string>
#include "File.h"

namespace gtrace
{
class FileWriter : public File
{
public:
    FileWriter() = default;
    FileWriter(const char* path);

    void writeLine(const char* data) const;
    void writeLine(const std::string& data) const { writeLine(data.c_str()); }
};
}  // namespace gtrace