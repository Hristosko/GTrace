#pragma once
#include <string>
#include "File.h"

namespace gtrace
{
class FileReader : public File
{
public:
    FileReader(const char* path);

    bool readLine(std::string* res);

protected:
    static constexpr int maxLineLength = 128;
    char buffer[maxLineLength];
};
}  // namespace gtrace