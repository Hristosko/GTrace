#pragma once

#include<cstdint>
#include "Parser.h"

#define SCENE_DEFAULT_WIDTH 800
#define SCENE_DEFAULT_HEIGHT 600

#define SCENE_DEFAULT_BACKGROUND Vector3f(0.f)

struct SceneSettings : public SceneElement {
	uint32_t width;
	uint32_t height;
	Vector3f background;

	SceneSettings()
		: width(SCENE_DEFAULT_WIDTH),
		height(SCENE_DEFAULT_HEIGHT),
		background(SCENE_DEFAULT_BACKGROUND) {}

	void parse(std::unordered_map<std::string, std::string>& map) override {
		SceneParser& parser = getParser();
		parser.parseuint32AndStore(map, "width", this->width);
		parser.parseuint32AndStore(map, "height", this->height);
		parser.parseVector3fAndStore(map, "background", this->background);
	}
};
