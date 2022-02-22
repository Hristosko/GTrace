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

    static inline const std::string Image = "Image";
    static inline const std::string ImageVariane = "ImageVariance";

    void addOutput(const std::string& name, DataBuffer&& data);
    DataBuffer& getOutput(const std::string& name);
    const DataBuffer& getOutput(const std::string& name) const;

    size_t count() const { return outputs.size(); }

    ConstIterator begin() const { return outputs.begin(); }
    ConstIterator end() const { return outputs.end(); }

private:
    std::unordered_map<std::string, DataBuffer> outputs;
};
}  // namespace gtrace
