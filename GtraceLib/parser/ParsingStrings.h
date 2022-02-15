#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <iostream>

namespace gtrace
{
struct ParserContext
{
    const char* filePath;
    uint64_t line;

    std::string message() const;
};

class ParsingStrings
{
public:
    static bool endsWith(const std::string& str, const std::string& ending)
    {
        return std::equal(ending.rbegin(), ending.rend(), str.rbegin());
    }

    static bool startsWith(const std::string& str, const std::string& start)
    {
        return std::equal(start.begin(), start.end(), str.begin());
    }

    static std::vector<std::string_view> split(std::string_view str, char delimer);
    static std::vector<std::string_view> split(std::string_view str, const std::string& delimer);

    static float parseFloat(std::string_view str, const ParserContext& context);
    static long long parseNumber(std::string_view str, const ParserContext& context, int base = 10);
};
}  // namespace gtrace
