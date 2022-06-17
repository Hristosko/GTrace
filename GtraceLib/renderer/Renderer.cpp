#include "Renderer.h"
#include <common/Color.h>

namespace gtrace
{
Renderer::Renderer(Scene scene, RendererOutput* output, UpdateRenderSurfaceFunc updateRenderSurfaceFunc) :
    scene(std::move(scene)),
    output(output),
    updateRenderSurfaceFunc(updateRenderSurfaceFunc)
{
    const auto sceneSize = scene.sceneSettings.width * scene.sceneSettings.height;
    output->reset();
    output->setSize(scene.sceneSettings.width, scene.sceneSettings.height);
    addOutput<Color3f>(RendererOutput::Image, sceneSize);
    addOutput<Color3f>(RendererOutput::ImageVariane, sceneSize);
}

void Renderer::render()
{
    for (int i = 0; i < scene.sceneSettings.height; ++i)
        renderLine(i);
    updateRenderSurfaceFunc();
}

void Renderer::renderLine(uint64_t line)
{
    const auto position = line * scene.sceneSettings.width;
    auto* imageOutput = output->getOutput(RendererOutput::Image).getAt<Color3f>(position);
    auto* varianceOutput = output->getOutput(RendererOutput::ImageVariane).getAt<Color3f>(position);

    for (uint64_t i = 0; i < scene.sceneSettings.width; ++i)
    {
        imageOutput[i] = Color3f(1.f, 0.f, 0.f);
        varianceOutput[i] = Color3f(0.f, 1.f, 0.f);
    }
    updateRenderSurfaceFunc();
}

}  // namespace gtrace
