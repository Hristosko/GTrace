#pragma once
#include "renderer/RendererOutput.h"

namespace gtrace
{
class GTRFile
{
public:
    static RendererOutput parse(const char* filePath);
    static void dump(const RendererOutput& output, const char* filePath);

private:
    static constexpr uint32_t CURRENT_VERSION = 1;
};
}  // namespace gtrace