#pragma warning(disable : 4996)

#include "Renderer.h"
#include "../Events.h"
#include "../Logger.h"

#include "../scene/World.h"

void Renderer::render() {
	LOGINFO("Start rendering.");
	World& w = getWorld();
	
	DataBuffer& buffer = this->output.getOutput(RendererOutputType::Image);
	uint32_t height = this->output.getHeight();
	uint32_t width = this->output.getWidth();
	char* pixelData = reinterpret_cast<char*>(buffer.getBuffer());
	for (uint32_t y = 0; y < height; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
			Ray ray;
			ray.origin = Vector3f(y, x, -10);
			ray.direction = Vector3f(0, 0, 1);

			HitRecort rec;
			rec.color = Vector3f(0.f);
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
