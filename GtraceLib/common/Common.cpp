#include <cstring>
#include "Common.h"

namespace gtrace
{
std::string_view getFileName(const char* fullPath)
{
    const auto length = strlen(fullPath);
    const char* end = fullPath + length;
    const char* start = end - 1;
    while (*start != '/' && *start != '\\')
        --start;

    ++start;
    return std::string_view(start, end - start);
}

}  // namespace gtrace