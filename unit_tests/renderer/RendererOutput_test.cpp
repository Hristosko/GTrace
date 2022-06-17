#include "TestUtils.h"
#include "common/Errors.h"
#include "renderer/RendererOutput.h"

using namespace gtrace;
class RendererOutputTest : public Test
{
public:
    void TearDown() override { output.reset(); }

    RendererOutput output;
};

TEST_F(RendererOutputTest, ReadOutputThrowIfNoOutput)
{
    EXPECT_THROW(output.getOutput(RendererOutput::Image), NotFoundError);
}

TEST_F(RendererOutputTest, AddOutput)
{
    output.addOutput(RendererOutput::Image, DataBuffer());
    EXPECT_NO_THROW(output.getOutput(RendererOutput::Image));
}

TEST_F(RendererOutputTest, AddOutputDuplicated)
{
    output.addOutput(RendererOutput::Image, DataBuffer());
    EXPECT_THROW(output.addOutput(RendererOutput::Image, DataBuffer()), DuplicatedError);
}

TEST_F(RendererOutputTest, Size)
{
    const auto width = 1;
    const auto height = 2;
    output.setSize(width, height);

    EXPECT_EQ(width, output.getWidth());
    EXPECT_EQ(height, output.getHeight());
}

TEST_F(RendererOutputTest, Reset)
{
    const auto width = 1;
    const auto height = 2;

    EXPECT_EQ(0, output.getWidth());
    EXPECT_EQ(0, output.getHeight());
    EXPECT_EQ(0, output.count());

    output.setSize(width, height);
    output.addOutput(RendererOutput::Image, DataBuffer());

    EXPECT_EQ(width, output.getWidth());
    EXPECT_EQ(height, output.getHeight());
    EXPECT_EQ(1, output.count());

    output.reset();

    EXPECT_EQ(0, output.getWidth());
    EXPECT_EQ(0, output.getHeight());
    EXPECT_EQ(0, output.count());
}
