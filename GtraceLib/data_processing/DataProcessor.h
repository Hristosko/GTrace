#include "data_structures/DataBuffer.h"

namespace gtrace
{
class DataProcessor
{
public:
    virtual void process(DataBuffer* data) const {}

protected:
    template<typename T>
    static float scalar(const T& v);
};

template<typename T>
float DataProcessor::scalar(const T& v)
{
    return v;
}

template<>
float DataProcessor::scalar(const Vector3f& v)
{
    return v.length();
}

}  // namespace gtrace
