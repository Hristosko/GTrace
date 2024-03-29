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

class BinFileWriter : public File
{
public:
    BinFileWriter() = default;
    BinFileWriter(const char* path);

    template<typename T>
    void write(const T* data, size_t count) const
    {
        write(data, sizeof(T), count);
    }

    void write(const void* data, size_t size, size_t count) const;
};
}  // namespace gtrace