#include "TestUtils.h"
#include "data_structures/DataBuffer.h"
#include "math/Vector3f.h"

using namespace gtrace;

class DataBufferTest : public Test
{
public:
    void SetUp() override { buffer = DataBuffer(); }

    DataBuffer buffer;
};

TEST_F(DataBufferTest, isInitialized)
{
    ASSERT_FALSE(buffer.isInitialized());
    buffer.init<int>(5);
    ASSERT_TRUE(buffer.isInitialized());
}

TEST_F(DataBufferTest, getAt)
{
    buffer.init<int>(5);
    const int val = 227;
    const int idx = 2;
    *buffer.getAt<int>(idx) = val;
    ASSERT_EQ(val, *buffer.getAt<int>(idx));
}

TEST_F(DataBufferTest, getAtVector3f)
{
    buffer.init<Vector3f>(5);
    const Vector3f val(227, 26, 42);
    const int idx = 2;
    *buffer.getAt<Vector3f>(idx) = val;
    ASSERT_EQ(val, *buffer.getAt<Vector3f>(idx));
}

TEST_F(DataBufferTest, getAtAlignofHugeValue)
{
    constexpr int alignment = 256;
    struct alignas(alignment) AlignedInt
    {
        uint32_t i;
    };
    struct alignas(alignment) AlignedFloat
    {
        float f;
    };

    buffer.init<AlignedInt>(5);
    const uint32_t val = 227;
    const int idx = 2;
    buffer.getAt<AlignedInt>(idx)->i = val;
    ASSERT_EQ(val, buffer.getAt<AlignedInt>(idx)->i);
    const float asFloat = reinterpret_cast<const float&>(val);
    ASSERT_EQ(asFloat, buffer.getAt<AlignedFloat>(idx)->f);
}

TEST_F(DataBufferTest, putAt)
{
    constexpr int N = 30;
    buffer.init<int>(N);
    int data[N];
    for (int i = 0; i < N; ++i)
        data[i] = 1;

    buffer.putAt(0, data, N);

    for (int i = 0; i < N / 3; ++i)
        data[i] = 2;
    buffer.putAt(N / 3, data, N / 3);

    int* res = buffer.getAt<int>(0);
    for (int i = 0; i < N; ++i)
    {
        if (i < N / 3 || i >= 2 * N / 3)
            EXPECT_EQ(1, res[i]);
        else
            EXPECT_EQ(2, res[i]);
    }
}

TEST_F(DataBufferTest, size)
{
    constexpr int N = 30;
    buffer.init<int>(N);
    EXPECT_EQ(sizeof(int), buffer.getElementSize());
    EXPECT_EQ(N, buffer.getSize());
}
