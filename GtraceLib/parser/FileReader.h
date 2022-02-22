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

    template<typename T>
    void read(T* buffer, size_t count) const
    {
        read(buffer, sizeof(T), count);
    }

    template<typename T>
    T read() const
    {
        T value;
        read(&value, sizeof(T), 1);
        return value;
    }

    void read(void* buffer, size_t size, size_t count) const;
};
}  // namespace gtrace