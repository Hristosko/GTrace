#include <math.h>
#include "RendererDisplay.h"
#include "common/Color.h"

namespace gtrace
{
RendererDisplay::RendererDisplay() : curDisplayType(RendererOutput::Image) {}

char* RendererDisplay::getPixels()
{
    return pixels.get();
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
    const auto width = output->getWidth();
    const auto height = output->getHeight();
    if (curDisplayType == RendererOutput::Image)
    {
        const auto size = width * height;
        pixels = std::make_unique<char[]>(3 * size);
        auto* data = output->getOutput(RendererOutput::Image).getAt<Color3f>(0);
        for (uint64_t i = 0; i < size; ++i)
        {
            pixels[3 * i] = static_cast<char>(255.f * data[i].x());
            pixels[3 * i + 1] = static_cast<char>(255.f * data[i].y());
            pixels[3 * i + 2] = static_cast<char>(255.f * data[i].z());
        }
    }
    if (curDisplayType == RendererOutput::ImageVariane)
    {
        const auto size = 3 * width * height;

        pixels = std::make_unique<char[]>(size);
        auto* data = output->getOutput(RendererOutput::ImageVariane).getAt<float>(0);
        for (uint64_t i = 0; i < size; ++i)
            pixels[i] = static_cast<char>(255.f * sqrtf(data[i]));
    }
}

}  // namespace gtrace
