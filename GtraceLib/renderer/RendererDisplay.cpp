#include <math.h>
#include "RendererDisplay.h"
#include "common/Color.h"
#include "data_processing/ColorToPixelProcessor.h"

namespace gtrace
{
RendererDisplay::RendererDisplay() : curDisplayType(RendererOutput::Image) {}

const DataBuffer& RendererDisplay::getPixels() const
{
    return pixels;
}

void RendererDisplay::setDisplayType(const RendererOutput::Type& type)
{
    if (type == curDisplayType)
        return;
    curDisplayType = type;
    updateDisplay();
}

void RendererDisplay::updateDisplay()
{
    if (curDisplayType == RendererOutput::Image)
    {
        pixels = output->getOutput(curDisplayType);
        ColorToPixelProcessor().process(&pixels);
    }
    if (curDisplayType == RendererOutput::ImageVariane)
    {
        pixels = output->getOutput(curDisplayType);
        ColorToPixelProcessor().process(&pixels);
    }
}

}  // namespace gtrace
