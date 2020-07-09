#include "RendererOutput.h"
#include "../scene/World.h"
#include "../Logger.h"
#include "../FileUtils.h"
#include "../Errors.h"

#define CURRENT_VERSION UINT32_C(1)

void RendererOutput::init() {
	this->initImageOutput();
	this->initVarianceOutput();
}

void RendererOutput::save(const char* path) {
	FILE* fp;
	if (0 != fopen_s(&fp, path, "wb")) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	FileRAII fpRAII(fp);
	const uint32_t version = CURRENT_VERSION;
	writeBin(&version, sizeof(version), 1, fp);
	
	// write image size
	const uint32_t width = getWorld().getSettings().width;
	const uint32_t height = getWorld().getSettings().height;

	writeBin(&width, sizeof(uint32_t), 1, fp);
	writeBin(&height, sizeof(uint32_t), 1, fp);

	uint16_t cnt = RendererOutputType::Count;
	writeBin(&cnt, sizeof(cnt), 1, fp);

	// write image
	cnt = RendererOutputType::Image;
	writeBin(&cnt, sizeof(cnt), 1, fp);
	writeBin(outputs[RendererOutputType::Image].getBuffer(), sizeof(ColorResult), width * height, fp);

	//write variance
	cnt = RendererOutputType::Variance;
	writeBin(&cnt, sizeof(cnt), 1, fp);
	writeBin(outputs[RendererOutputType::Variance].getBuffer(), sizeof(VarianceResult), width * height, fp);
}

void RendererOutput::open(const char* path) {
	FILE* fp;
	if (0 != fopen_s(&fp, path, "rb")) {
		LOGERROR("Cannot open file: ", path);
		return;
	}
	uint32_t version; // ignored
	size_t s;
	s = fread(&version, sizeof(version), 1, fp);
	LOGINFO(s);
	LOGINFO("opening grt file, version: ", version);
	// write image size
	uint32_t width;
	uint32_t height;

	s = fread(&width, sizeof(width), 1, fp);
	LOGINFO(s);
	s = fread(&height, sizeof(height), 1, fp);
	LOGINFO(s);
	LOGINFO("scene width: ", width, " scene height: ", height);
	getWorld().getSettings().width = width;
	getWorld().getSettings().height = height;
	uint32_t size = width * height;
	this->init();

	uint16_t cnt;
	bool fail = false;
	s = fread(&cnt, sizeof(cnt), 1, fp);
	LOGINFO(s);
	LOGINFO("output count: ", cnt);
	for (uint16_t i = 0; i < cnt; ++i && !fail) {
		uint16_t type;
		s = fread(&type, sizeof(type), 1, fp);
		LOGINFO(s);
		LOGINFO("new render output: ", type);
		switch (type)
		{
		case RendererOutputType::Image:
			s = 0;
			s = fread(this->getOutput(RendererOutputType::Image).getBuffer(),
									sizeof(ColorResult), size, fp);
			LOGINFO(s);
			break;
		case RendererOutputType::Variance:
			s = fread(this->getOutput(RendererOutputType::Variance).getBuffer(),
				sizeof(VarianceResult), size, fp);
			LOGINFO(s);
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
