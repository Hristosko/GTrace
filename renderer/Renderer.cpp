#pragma warning(disable : 4996)

#include "Renderer.h"
#include "../Events.h"
#include "../Logger.h"

void Renderer::render() {
	LOGINFO("Start rendering.");
	
	DataBuffer& buffer = this->output.getOutput(RendererOutputType::Image);
	uint32_t height = this->output.getHeight();
	uint32_t width = this->output.getWidth();
	char* pixelData = reinterpret_cast<char*>(buffer.getBuffer());
	for (uint32_t y = 0; y < height; ++y)
	{
		for (uint32_t x = 0; x < width; ++x)
		{
			pixelData[3 * y * width + 3 * x] = (char)255;
			pixelData[3 * y * width + 3 * x + 1] = 0;
			pixelData[3 * y * width + 3 * x + 2] = 0;
		}
	}

	LOGINFO("Finish rendering.")

	wxCommandEvent event(GTRACE_RENDERED_ELEMENT);
	wxPostEvent(this->renderSurface, event);
}
