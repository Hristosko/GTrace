#pragma once

#include "RendererOutput.h"
#include <memory>

namespace gtrace
{
class RendererDisplay
{
public:
    RendererDisplay();

    void setOutput(const RendererOutput* output) { this->output = output; }
    char* getPixels();

    void setDisplayType(const RendererOutput::Type& type);
    void updateDisplay();

private:
    RendererOutput::Type curDisplayType;
    const RendererOutput* output;
    std::unique_ptr<char[]> pixels;
};

}  // namespace gtrace
