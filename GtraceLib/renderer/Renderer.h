#pragma once
#include <functional>
#include "Scene.h"
#include "RendererOutput.h"

namespace gtrace
{
class Renderer
{
public:
    using UpdateRenderSurfaceFunc = std::function<void()>;
    Renderer(Scene scene, RendererOutput* output, UpdateRenderSurfaceFunc updateRenderSurfaceFunc);

    void render();

private:
    template<typename OutputType>
    void addOutput(const std::string& outputName, uint64_t outputSize)
    {
        DataBuffer buffer;
        buffer.init<OutputType>(outputSize);
        output->addOutput(outputName, std::move(buffer));
    }

    void renderLine(uint64_t line);

    const RendererOutput& getOutput() const { return *output; }

private:
    Scene scene;
    RendererOutput* output;
    UpdateRenderSurfaceFunc updateRenderSurfaceFunc;
};
}  // namespace gtrace