#pragma warning(disable : 4996)

#include "Renderer.h"
#include "../Events.h"
#include "../Logger.h"

#include "../scene/World.h"

#include<iostream>
void Renderer::render() {
	LOGINFO("Start rendering.");
	World& w = getWorld();
	
	DataBuffer& buffer = this->output.getOutput(RendererOutputType::Image);
	const SceneSettings& settings = w.getSettings();
	uint32_t height = settings.height;
	uint32_t width = settings.width;
	std::cout << height << " " << width << std::endl;
	char* pixelData = reinterpret_cast<char*>(buffer.getBuffer());
	for (uint32_t y = 0; y < height; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
			Ray ray;
			ray.origin = Vector3f(x, y, -10);
			ray.direction = Vector3f(0, 0, 1);

			HitRecort rec;
			rec.color = settings.background;
			rec.t = 1000000.f;
			for (Shape* shape : w.getShapes()) {
				shape->hit(ray, 0.f, rec.t, 0, rec);
			}
			pixelData[3 * y * width + 3 * x] = (char)(255*rec.color.x());
			pixelData[3 * y * width + 3 * x + 1] = (char)(255 * rec.color.y());
			pixelData[3 * y * width + 3 * x + 2] = (char)(255 * rec.color.z());
		}
	}

	LOGINFO("Finish rendering.")

	wxCommandEvent event(GTRACE_RENDERED_ELEMENT);
	wxPostEvent(this->renderSurface, event);
}
