#include "TestUtils.h"
#include "parser/GTRFile.h"

using namespace gtrace;
class GTRFileTest : public Test
{
public:
    static constexpr auto* file = "test.gtr";
    static constexpr uint64_t DATA_BUFFER_SIZE = 10;

    void TearDown() override { remove(file); }

    template<typename T, typename U>
    static RendererOutput generateOutput(const T& t, const U& u)
    {
        RendererOutput output;
        output.addOutput(RendererOutput::Image, generateDataBuffer(t));
        output.addOutput(RendererOutput::ImageVariane, generateDataBuffer(u));

        return output;
    }

    template<typename T>
    static DataBuffer generateDataBuffer(const T& t)
    {
        DataBuffer buffer;
        buffer.init<T>(DATA_BUFFER_SIZE);
        T* data = buffer.getAt<T>(0);

        for (uint64_t i = 0; i < DATA_BUFFER_SIZE; ++i)
            data[i] = t;

        return buffer;
    }
};

TEST_F(GTRFileTest, DumpAndParseFileBasic)
{
    const auto output = generateOutput(int{1}, float{2.f});
    GTRFile::dump(output, file);

    const auto parsedOutput = GTRFile::parse(file);

    EXPECT_EQ(output.count(), parsedOutput.count());
    for (const auto& [name, data] : output)
    {
        const auto& parsedData = parsedOutput.getOutput(name);
        EXPECT_EQ(data, parsedData);
    }
}

TEST_F(GTRFileTest, DumpAndParseFileVector3f)
{
    const auto output = generateOutput(Vector3f{26.f}, Vector3f{0.227f});
    GTRFile::dump(output, file);

    const auto parsedOutput = GTRFile::parse(file);

    EXPECT_EQ(output.count(), parsedOutput.count());
    for (const auto& [name, data] : output)
    {
        const auto& parsedData = parsedOutput.getOutput(name);
        EXPECT_EQ(data, parsedData);
    }
}
