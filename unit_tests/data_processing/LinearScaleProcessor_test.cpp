#include "TestUtils.h"
#include "common/Color.h"
#include "data_processing/LinearScaleProcessor.h"

using namespace gtrace;

using TestTypes = testing::Types<float, Color3f>;

template<typename T>
class LinearScaleProcessorTest : public Test
{
};

TYPED_TEST_SUITE(LinearScaleProcessorTest, TestTypes);

TYPED_TEST(LinearScaleProcessorTest, LessThenFrom)
{
    DataBuffer buffer;
    LinearScaleProcessor<TypeParam> proc(1.f, 2.f);
    buffer.init<TypeParam>(1);
    buffer.at<TypeParam>(0) = TypeParam(0.f);

    proc.process(&buffer);
    EXPECT_EQ(0.f, buffer.at<float>(0));
}

TYPED_TEST(LinearScaleProcessorTest, LargerThenTo)
{
    DataBuffer buffer;
    LinearScaleProcessor<TypeParam> proc(0.f, 1.f);
    buffer.init<TypeParam>(1);
    buffer.at<TypeParam>(0) = TypeParam(1000.f);

    proc.process(&buffer);
    EXPECT_EQ(1.f, buffer.at<float>(0));
}

template<typename T>
T getValueFromScalar(float v)
{
    return v;
}

template<>
Color3f getValueFromScalar(float v)
{
    return Color3f(v / sqrtf(3.f));
}

TYPED_TEST(LinearScaleProcessorTest, IntermidiatateValues)
{
    DataBuffer buffer;
    LinearScaleProcessor<TypeParam> proc(0.f, 1.f);
    std::vector<float> values = {0.02f, 0.1f, 0.4f, 0.666f, 0.75f, 0.82f, 0.95f};
    buffer.init<TypeParam>(values.size());
    for (size_t i = 0; i < values.size(); ++i)
        buffer.at<TypeParam>(i) = getValueFromScalar<TypeParam>(values[i]);

    proc.process(&buffer);
    EXPECT_EQ(buffer.getSize(), values.size());
    for (size_t i = 0; i < values.size(); ++i)
        EXPECT_NEAR(buffer.at<float>(i), values[i], EPS);
}

TYPED_TEST(LinearScaleProcessorTest, IntermidiatateValuesScaled)
{
    DataBuffer buffer;
    const float scale = 26.f;
    LinearScaleProcessor<TypeParam> proc(0.f, scale);
    std::vector<float> values = {0.02f, 0.1f, 0.4f, 0.666f, 0.75f, 0.82f, 0.95f};

    buffer.init<TypeParam>(values.size());
    for (size_t i = 0; i < values.size(); ++i)
        buffer.at<TypeParam>(i) = getValueFromScalar<TypeParam>(values[i] * scale);

    proc.process(&buffer);
    EXPECT_EQ(buffer.getSize(), values.size());
    for (size_t i = 0; i < values.size(); ++i)
        EXPECT_NEAR(buffer.at<float>(i), values[i], EPS);
}

TYPED_TEST(LinearScaleProcessorTest, IntermidiatateValuestTranslated)
{
    DataBuffer buffer;
    const float from = 26.f;
    const float to = 227.f;
    LinearScaleProcessor<TypeParam> proc(from, to);
    std::vector<float> values = {0.02f, 0.1f, 0.4f, 0.666f, 0.75f, 0.82f, 0.95f};

    buffer.init<TypeParam>(values.size());
    for (size_t i = 0; i < values.size(); ++i)
        buffer.at<TypeParam>(i) = getValueFromScalar<TypeParam>(values[i] * (to - from) + from);

    proc.process(&buffer);
    EXPECT_EQ(buffer.getSize(), values.size());
    for (size_t i = 0; i < values.size(); ++i)
        EXPECT_NEAR(buffer.at<float>(i), values[i], EPS);
}
