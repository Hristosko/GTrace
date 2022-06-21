#include <algorithm>
#include <math/Vector3f.h>
#include "DataProcessor.h"

namespace gtrace
{
template<typename T>
class LinearScaleProcessor : public DataProcessor
{
public:
    LinearScaleProcessor(float from, float to) : from(from), to(to), range(to - from) { assert(to > from); }

    virtual void process(DataBuffer* data) const
    {
        assert(data && sizeof(T) == data->getElementSize());

        const auto input = data->getAt<T>(0);
        DataBuffer result;
        result.init<float>(data->getSize());
        auto output = result.getAt<float>(0);

        for (uint64_t i = 0; i < data->getSize(); ++i)
            output[i] = getScale(input[i]);

        std::swap(*data, result);
    }

private:
    float getScale(const T& v) const
    {
        const auto value = scalar(v);
        const auto scale = (value - from) / range;
        return std::clamp(scale, 0.f, 1.f);
    }

private:
    float from;
    float to;
    float range;
};

}  // namespace gtrace