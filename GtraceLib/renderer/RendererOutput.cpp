#include "RendererOutput.h"
#include "scene/World.h"
#include "Logger.h"
#include "FileUtils.h"
#include "Errors.h"

#include<assert.h>

namespace gtrace {

#define CURRENT_VERSION UINT32_C(1)

void gtrace::RendererOutput::OutputBuffer::init(uint32_t w, uint32_t h) {
	{
		DataBuffer& buffer = outputs[RendererOutputType::Image];
		buffer.init<ColorResult>((uint64_t)w * h);
	}
	{
		DataBuffer& buffer = outputs[RendererOutputType::Variance];
		buffer.init<VarianceResult>((uint64_t)w* h);
	}
}

void RendererOutput::init() {
	const SceneSettings& settings = this->world.getSettings();
	this->buff.init(settings.width, settings.height);
}

void RendererOutput::save(const char* path) {
	FILE* fp = fopen(path, "wb");
	if (fp == nullptr) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	FileRAII fpRAII(fp);
	const uint32_t version = CURRENT_VERSION;
	writeBin(&version, sizeof(version), 1, fp);

	// write image size
	const uint32_t width = this->world.getSettings().width;
	const uint32_t height = this->world.getSettings().height;

	writeBin(&width, sizeof(uint32_t), 1, fp);
	writeBin(&height, sizeof(uint32_t), 1, fp);

	uint16_t cnt = RendererOutputType::Count;
	writeBin(&cnt, sizeof(cnt), 1, fp);

	// write image
	cnt = RendererOutputType::Image;
	writeBin(&cnt, sizeof(cnt), 1, fp);
	writeBin(buff.outputs[RendererOutputType::Image].getBuffer(), sizeof(ColorResult), width * height, fp);

	//write variance
	cnt = RendererOutputType::Variance;
	writeBin(&cnt, sizeof(cnt), 1, fp);
	writeBin(buff.outputs[RendererOutputType::Variance].getBuffer(), sizeof(VarianceResult), width * height, fp);
}

void RendererOutput::open(const char* path) {
	FILE* fp = fopen(path, "rb");
	if (fp == nullptr) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	uint32_t version; // ignored
	readBin(&version, sizeof(version), 1, fp);
	LOGINFO("opening grt file, version: ", version);
	// write image size
	uint32_t width;
	uint32_t height;

	readBin(&width, sizeof(width), 1, fp);
	readBin(&height, sizeof(height), 1, fp);
	LOGINFO("scene width: ", width, " scene height: ", height);
	this->world.getSettings().width = width;
	this->world.getSettings().height = height;
	uint32_t size = width * height;
	this->init();

	uint16_t cnt;
	bool fail = false;
	readBin(&cnt, sizeof(cnt), 1, fp);
	LOGINFO("output count: ", cnt);
	for (uint16_t i = 0; i < cnt; ++i && !fail) {
		uint16_t type;
		readBin(&type, sizeof(type), 1, fp);
		LOGINFO("new render output: ", type);
		switch (type)
		{
		case RendererOutputType::Image:
			readBin(this->getOutput(RendererOutputType::Image).getBuffer(),
									sizeof(ColorResult), size, fp);
			break;
		case RendererOutputType::Variance:
			readBin(this->getOutput(RendererOutputType::Variance).getBuffer(),
				sizeof(VarianceResult), size, fp);
			break;
		default:
			LOGERROR("Unknown render output.");
			fail = true;
			break;
		}
	}
}

RendererOutput::OutputBuffer RendererOutput::getOutputBuffer(uint32_t w, uint32_t h) {
	OutputBuffer buff;
	buff.init(w, h);
	return buff;
}

void RendererOutput::update(const OutputBuffer& b,
	uint32_t offsetw, uint32_t offseth,
	uint32_t sizew, uint32_t sizeh) {

	const uint32_t worldWidth = this->world.getSettings().width;
	for (uint32_t ih = 0; ih < sizeh; ++ih) {
		const uint64_t realih = (uint64_t)ih + offseth;
		assert(realih < this->world.getSettings().height);
		for (uint32_t iw = 0; iw < sizew; ++iw) {
			const uint64_t realiw = (uint64_t)iw + offsetw;
			assert(realiw < this->world.getSettings().width);

			const uint64_t pos = (uint64_t)ih * worldWidth + iw;
			const uint64_t srcPos = (uint64_t)ih * offsetw + iw;

			this->buff.outputs[RendererOutputType::Image].copyValue<ColorResult>(pos, b.outputs[RendererOutputType::Image], srcPos);
			this->buff.outputs[RendererOutputType::Variance].copyValue<VarianceResult>(pos, b.outputs[RendererOutputType::Variance], srcPos);
		}
	}
}

}
