#include "TestUtils.h"
#include "common/Errors.h"
#include "parser/ParsingStrings.h"

using namespace gtrace;

static inline const std::string quote =
    "A theory with mathematical beauty is more likely to be correct than an ugly one that fits some experimental "
    "data.";

static inline const std::string author = "Paul Dirac";

static inline const ParserContext testContext{nullptr, 0};

TEST(ParsingStrings, startsWith)
{
    ASSERT_TRUE(ParsingStrings::startsWith(author + quote, author));
    ASSERT_TRUE(ParsingStrings::startsWith(quote + author, quote));
    ASSERT_NE(author.size(), quote.size());
    ASSERT_FALSE(ParsingStrings::startsWith(author, quote));
    ASSERT_FALSE(ParsingStrings::startsWith(quote, author));
}

TEST(ParsingStrings, endsWith)
{
    ASSERT_TRUE(ParsingStrings::endsWith(author + quote, quote));
    ASSERT_TRUE(ParsingStrings::endsWith(quote + author, author));
    ASSERT_NE(author.size(), quote.size());
    ASSERT_FALSE(ParsingStrings::endsWith(author, quote));
    ASSERT_FALSE(ParsingStrings::endsWith(quote, author));
}

TEST(ParsingStrings, split)
{
    constexpr int count = 10;
    const std::string chunk = "AAA";
    constexpr char delimer = ' ';

    std::string testString;
    testString.reserve(count * (chunk.size() + 1));
    for (int i = 0; i < count - 1; ++i)
    {
        testString += chunk;
        testString += delimer;
    }
    testString += chunk;

    const auto chunks = ParsingStrings::split(testString, delimer);

    EXPECT_EQ(count, chunks.size());
    for (const auto& x : chunks)
        ASSERT_EQ(chunk, x);
}

TEST(ParsingStrings, splitEndsWithDelimer)
{
    const std::string chunk = "AAA";
    constexpr char delimer = ' ';

    const auto chunks = ParsingStrings::split(chunk + delimer, delimer);
    ASSERT_EQ(1, chunks.size());
    ASSERT_EQ(chunk, chunks[0]);
}

TEST(ParsingStrings, splitString)
{
    constexpr int count = 10;
    const std::string chunk = "AAA";
    const std::string delimer = "BBB";

    std::string testString;
    testString.reserve(count * (chunk.size() + delimer.size()));
    for (int i = 0; i < count; ++i)
    {
        testString += chunk;
        testString += delimer;
    }

    const auto chunks = ParsingStrings::split(testString, delimer);

    EXPECT_EQ(count, chunks.size());
    for (const auto& x : chunks)
        ASSERT_EQ(chunk, x);
}

TEST(ParsingStrings, parseFloat)
{
    const float v = 227.4226;
    std::string floatStr = std::to_string(v);

    EXPECT_NEAR(v, ParsingStrings::parseFloat(floatStr, testContext), EPS);

    floatStr += "f";
    EXPECT_THROW(ParsingStrings::parseFloat(floatStr, testContext), ParserError);
}

TEST(ParsingStrings, parseNumber)
{
    const int v = -227;
    std::string str = std::to_string(v);

    EXPECT_EQ(v, ParsingStrings::parseNumber(str, testContext));

    str += "f";
    EXPECT_THROW(ParsingStrings::parseNumber(str, testContext), ParserError);
}

TEST(ParsingStrings, parseNumberFromFloat)
{
    const float v = 227.4226;
    const auto str = std::to_string(v);
    EXPECT_THROW(ParsingStrings::parseNumber(str, testContext), ParserError);
}

TEST(ParsingStrings, parseNumberBase16)
{
    const int v = 0xFA26;
    const auto str = "0xFA26";

    EXPECT_EQ(v, ParsingStrings::parseNumber(str, testContext, 16));
}
