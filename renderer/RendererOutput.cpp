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
	fwrite(outputs[RendererOutputType::Variance].getBuffer(), sizeof(VarianceResult), width * height, fp);

	fclose(fp);
}

void RendererOutput::open(const char* path) {
	FILE* fp;
	if (0 != fopen_s(&fp, path, "r")) {
		LOGERROR("Cannot open file: ", path);
		return;
	}
	uint32_t version; // ignored
	fread(&version, sizeof(version), 1, fp);
	LOGINFO("opening grt file, version: ", version);
	// write image size
	uint32_t width;
	uint32_t height;

	fread(&width, sizeof(width), 1, fp);
	fread(&height, sizeof(height), 1, fp);
	LOGINFO("scene width: ", width, " scene height: ", height);
	getWorld().getSettings().width = width;
	getWorld().getSettings().height = height;
	uint32_t size = width * height;
	this->init();

	uint16_t cnt;
	bool fail = false;
	fread(&cnt, sizeof(cnt), 1, fp);
	LOGINFO("output count: ", cnt);
	for (uint16_t i = 0; i < cnt; ++i && !fail) {
		uint16_t type;
		fread(&type, sizeof(type), 1, fp);
		LOGINFO("new render output: ", type);
		switch (type)
		{
		case RendererOutputType::Image:
			fread(this->getOutput(RendererOutputType::Image).getBuffer(),
									sizeof(ColorResult), size, fp);
			break;
		case RendererOutputType::Variance:
			fread(this->getOutput(RendererOutputType::Variance).getBuffer(),
				sizeof(VarianceResult), size, fp);
			break;
		default:
			LOGERROR("Unknown render output.");
			fail = true;
			break;
		}
	}

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
