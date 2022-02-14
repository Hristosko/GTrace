#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <iostream>

namespace gtrace
{
inline bool endsWith(const std::string& str, const std::string& ending)
{
    if (ending.size() > str.size())
        return false;
    return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
}

inline bool startsWith(const std::string& str, const std::string& start)
{
    if (start.size() > str.size())
        return false;
    return std::equal(start.begin(), start.end(), str.begin());
}

inline std::vector<std::string_view> split(const std::string& str, char delimer)
{
    std::vector<std::string_view> res;
    std::string::size_type pos = 0;
    for (;;)
    {
        const auto oldPos = pos;
        pos = str.find(delimer, pos);
        if (pos == std::string::npos)
        {
            if (oldPos != str.size() - 1)
                res.emplace_back(std::string_view(str.data() + oldPos, str.size() - oldPos));

            break;
        }

        res.emplace_back(std::string_view(str.data() + oldPos, pos - oldPos));
        ++pos;
    }
    return res;
}
}  // namespace gtrace
