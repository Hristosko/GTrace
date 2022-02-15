#include "TestUtils.h"
#include "common/Errors.h"
#include "parser/FileReader.h"
#include "parser/FileWriter.h"

using namespace gtrace;

class FileReaderTestable : public FileReader
{
public:
    static int getMaxLineLength() { return maxLineLength; }
};

class FileTest : public Test
{
public:
    void TearDown() override { remove(filePath); }

    static constexpr auto* filePath = "test_file";
    static inline const std::string testFilesDirPath = "unit_tests/parser/test_files/";
};

static void testFile(const std::string& path)
{
    FileReader reader(path.c_str());
    int cur = 1;
    std::string read;
    while (reader.readLine(&read))
    {
        ASSERT_EQ(cur, atoi(read.c_str()));
        ASSERT_EQ(read, std::to_string(cur));
        ++cur;
    }
}

TEST_F(FileTest, IsOpen)
{
    FileWriter writer(filePath);
    EXPECT_TRUE(writer.isOpen());
    writer.close();
    EXPECT_FALSE(writer.isOpen());
}

TEST_F(FileTest, ReadEmptyFile)
{
    EXPECT_THROW(FileReader reader(filePath), FileError);
}

TEST_F(FileTest, WriteAndReadLine)
{
    FileWriter writer(filePath);
    const std::string line =
        "A theory with mathematical beauty is more likely to be correct than an ugly one that fits some experimental "
        "data.";
    writer.writeLine(line.c_str());
    writer.close();
    EXPECT_FALSE(writer.isOpen());

    FileReader reader(filePath);
    ASSERT_FALSE(reader.isAtEnd());
    std::string read;
    auto status = reader.readLine(&read);
    ASSERT_EQ(line, read);
    ASSERT_TRUE(status);
    ASSERT_FALSE(reader.isAtEnd());
    status = reader.readLine(&read);
    ASSERT_FALSE(status);
    ASSERT_TRUE(reader.isAtEnd());
}

TEST_F(FileTest, WriteAdReadMultipleLines)
{
    const auto numLines = 10;
    {
        FileWriter writer(filePath);
        for (auto i = 1; i <= numLines; ++i)
            writer.writeLine(std::to_string(i).c_str());
    }

    testFile(filePath);
}

TEST_F(FileTest, LongLine)
{
    const int lineLength = FileReaderTestable::getMaxLineLength() + 26;
    const std::string line(lineLength, 'A');
    {
        FileWriter writer(filePath);
        writer.writeLine(line.c_str());
    }

    FileReader reader(filePath);
    std::string read;
    EXPECT_TRUE(reader.readLine(&read));
    EXPECT_EQ(line, read);
}

#ifndef WIN32

TEST_F(FileTest, LF_File)
{
    testFile(testFilesDirPath + "lf_file.txt");
}

TEST_F(FileTest, CRLF_File)
{
    testFile(testFilesDirPath + "crlf_file.txt");
}

#endif  // !WIN32