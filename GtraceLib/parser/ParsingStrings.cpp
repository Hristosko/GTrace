#include "ParsingStrings.h"
#include "common/Errors.h"

namespace gtrace
{
std::string ParserContext::message() const
{
    return (filePath == nullptr) ? "" : filePath + (':' + std::to_string(line)) + ": ";
}

static std::string::size_type getSize(char x)
{
    return 1;
}
static std::string::size_type getSize(const std::string& x)
{
    return x.size();
};

template<typename Delimer>
std::vector<std::string_view> splitImpl(std::string_view str, const Delimer& delimer)
{
    std::vector<std::string_view> res;
    const auto addSubstring = [&str, &res](std::string::size_type start, std::string::size_type end) {
        if (start != end)
            res.emplace_back(std::string_view(str.data() + start, end - start));
    };

    std::string::size_type pos = 0;
    for (;;)
    {
        const auto oldPos = pos;
        pos = str.find(delimer, pos);
        if (pos == std::string::npos)
        {
            addSubstring(oldPos, str.size());
            break;
        }

        addSubstring(oldPos, pos);
        pos += getSize(delimer);
    }
    return res;
}

std::vector<std::string_view> ParsingStrings::split(std::string_view str, char delimer)
{
    return splitImpl(str, delimer);
}

std::vector<std::string_view> ParsingStrings::split(std::string_view str, const std::string& delimer)
{
    return splitImpl(str, delimer);
}

float ParsingStrings::parseFloat(std::string_view str, const ParserContext& context)
{
    char* end;
    float res = std::strtof(str.data(), &end);
    if (end != str.data() + str.size())
        Raise(ParserError(context.message() + "Unable to parse float from: " + str.data()));
    return res;
}

long long ParsingStrings::parseNumber(std::string_view str, const ParserContext& context, int base)
{
    char* end;
    long long res = std::strtoll(str.data(), &end, base);
    if (end != str.data() + str.size())
        Raise(ParserError(context.message() + "Unable to parse number from: " + str.data()));
    return res;
}

}  // namespace gtrace