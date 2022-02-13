#include "TestUtils.h"
#include "parser/ParsedParams.h"
#include "common/Errors.h"

using namespace gtrace;

TEST(ParsedParams, throwIfNoParam)
{
    ParsedParams params;
    EXPECT_THROW(params.getFloat("A"), ParserError);
}

TEST(ParsedParams, getParam)
{
    const std::string name = "A";
    const int value = 1;
    ParsedParams params;
    params.addInt(name, value);
    EXPECT_EQ(value, params.getInt(name));
    EXPECT_THROW(params.getFloat(name), ParserError);
    EXPECT_THROW(params.getInt(name + name), ParserError);
}

TEST(ParsedParams, overwrite)
{
    const std::string name = "A";
    const int value = 1;
    const int newValue = 2;
    ParsedParams params;
    params.addInt(name, value);
    params.addInt(name, newValue);
    EXPECT_EQ(newValue, params.getInt(name));
}
