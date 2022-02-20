#pragma once
#include <string>
#include <unordered_map>
#include "data_structures/DataBuffer.h"

namespace gtrace
{
class RendererOutput
{
public:
    static inline const std::string Image = "Image";
    static inline const std::string ImageVariane = "ImageVariance";

    void addOutput(const std::string& name, DataBuffer&& data);
    DataBuffer& getOutput(const std::string& name);
    const DataBuffer& getOutput(const std::string& name) const;
private:
    std::unordered_map<std::string, DataBuffer> outputs;
};
}  // namespace gtrace
