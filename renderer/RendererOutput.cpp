#include "RendererOutput.h"
#include "../scene/World.h"
#include "../Logger.h"

#include <stdio.h>

#define CURRENT_VERSION UINT32_C(1)

void RendererOutput::init() {
	this->initImageOutput();
	this->initVarianceOutput();
}

void RendererOutput::save(const char* path) {
	FILE* fp;
	if (0 != fopen_s(&fp, path, "w")) {
		LOGERROR("Cannot open file: ", path);
		return;
	}
	const uint32_t version = CURRENT_VERSION;
	fwrite(&version, sizeof(version), 1, fp);

	// write image size
	const uint32_t width = getWorld().getSettings().width;
	const uint32_t height = getWorld().getSettings().height;

	fwrite(&width, sizeof(uint32_t), 1, fp);
	fwrite(&height, sizeof(uint32_t), 1, fp);
	uint16_t cnt = RendererOutputType::Count;
	fwrite(&cnt, sizeof(cnt), 1, fp);

	// write image
	cnt = RendererOutputType::Image;
	fwrite(&cnt, sizeof(cnt), 1, fp);
	fwrite(outputs[RendererOutputType::Image].getBuffer(), sizeof(ColorResult), width * height, fp);

	//write variance
	cnt = RendererOutputType::Variance;
	fwrite(&cnt, sizeof(cnt), 1, fp);
	fwrite(outputs[RendererOutputType::Variance].getBuffer(), sizeof(ColorResult), width * height, fp);

	fclose(fp);
}

void RendererOutput::initImageOutput() {
	const SceneSettings& settings = getWorld().getSettings();
	DataBuffer& buffer = this->getOutput(RendererOutputType::Image);
	buffer.init<ColorResult>((uint64_t)settings.width * settings.height);
}

void RendererOutput::initVarianceOutput() {
	const SceneSettings& settings = getWorld().getSettings();
	DataBuffer& buffer = this->getOutput(RendererOutputType::Variance);
	buffer.init<VarianceResult>((uint64_t)settings.width * settings.height);
}
