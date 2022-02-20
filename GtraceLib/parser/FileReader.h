#pragma once
#include <string>
#include "File.h"

namespace gtrace
{
class FileReader : public File
{
public:
    FileReader(const char* path);

    bool readLine(std::string* res) const;

protected:
    static constexpr int maxLineLength = 128;
    mutable char buffer[maxLineLength];
};

class BinFileReader : public File
{
public:
    BinFileReader(const char* path);

    void read(void* buffer, size_t size, size_t count) const;
};
}  // namespace gtrace