#pragma once
#include "DataProcessor.h"
#include "common/Color.h"
#include "common/Errors.h"

namespace gtrace
{
class ColorToPixelProcessor : DataProcessor
{
    using Pixel = unsigned char;

public:
    virtual void process(DataBuffer* data) const
    {
        if (data->getElementSize() != sizeof(Color3f))
            Raise(InvalidDataBuffer(
                "Expected Color3f buffer. Actual element size is: " + std::to_string(data->getElementSize())));

        DataBuffer result;
        result.init<Pixel>(3 * data->getSize());

        const auto* input = data->getAt<Color3f>(0);
        auto* output = result.getAt<Pixel>(0);

        for (uint64_t i = 0; i < data->getSize(); ++i)
        {
            output[3 * i] = static_cast<Pixel>(255.f * input[i].x());
            output[3 * i + 1] = static_cast<Pixel>(255.f * input[i].y());
            output[3 * i + 2] = static_cast<Pixel>(255.f * input[i].z());
        }

        std::swap(result, *data);
    }
};

}  // namespace gtrace