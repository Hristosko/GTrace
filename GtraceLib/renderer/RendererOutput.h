#pragma once
#include <string>
#include <unordered_map>
#include "data_structures/DataBuffer.h"

namespace gtrace
{
class RendererOutput
{
public:
    using ConstIterator = std::unordered_map<std::string, DataBuffer>::const_iterator;
    using Type = std::string;

    static inline const Type Image = "Image";
    static inline const Type ImageVariane = "ImageVariance";

    void setSize(uint64_t width, uint64_t height)
    {
        this->width = width;
        this->height = height;
    }

    uint64_t getWidth() const { return width; }
    uint64_t getHeight() const { return height; }

    void addOutput(const std::string& name, DataBuffer&& data);
    DataBuffer& getOutput(const std::string& name);
    const DataBuffer& getOutput(const std::string& name) const;

    size_t count() const { return outputs.size(); }

    ConstIterator begin() const { return outputs.begin(); }
    ConstIterator end() const { return outputs.end(); }

    void reset()
    {
        width = height = 0;
        outputs.clear();
    }

private:
    uint64_t width;
    uint64_t height;
    std::unordered_map<Type, DataBuffer> outputs;
};
}  // namespace gtrace
