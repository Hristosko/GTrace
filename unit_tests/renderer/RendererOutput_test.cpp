#include "TestUtils.h"
#include "common/Errors.h"
#include "renderer/RendererOutput.h"

using namespace gtrace;
class RendererOutputTest : public Test
{
public:
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
