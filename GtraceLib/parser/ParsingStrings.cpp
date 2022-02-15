#include "ParsingStrings.h"
#include "common/Errors.h"

namespace gtrace
{
static std::string contextMessage(const ParserContext& context)
{
    return (context.filePath == nullptr) ? "" : context.filePath + std::to_string(context.line) + ": ";
}

std::vector<std::string_view> ParsingStrings::split(const std::string& str, char delimer)
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
            if (oldPos != str.size() - 1)
                addSubstring(oldPos, str.size());
            break;
        }

        addSubstring(oldPos, pos++);
    }
    return res;
}

float ParsingStrings::parseFloat(std::string_view str, const ParserContext& context)
{
    char* end;
    float res = std::strtof(str.data(), &end);
    if (end != str.data() + str.size())
        Raise(ParserError(contextMessage(context) + "Unable to parse float from: " + str.data()));
    return res;
}

}  // namespace gtrace