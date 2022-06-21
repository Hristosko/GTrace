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
    const DataBuffer& getPixels() const;

    void setDisplayType(const RendererOutput::Type& type);
    void updateDisplay();

private:
    RendererOutput::Type curDisplayType;
    const RendererOutput* output;
    DataBuffer pixels;
};

}  // namespace gtrace
