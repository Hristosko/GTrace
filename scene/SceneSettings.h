#pragma once

#include<cstdint>
#include "Parser.h"

#define SCENE_DEFAULT_WIDTH 800
#define SCENE_DEFAULT_HEIGHT 600
#define SCENE_DEFAULT_SUBDIVS 4

#define SCENE_DEFAULT_BACKGROUND Vector3f(0.f)

struct SceneSettings : public SceneElement {
	uint32_t width;
	uint32_t height;
	uint32_t subdivs;
	Vector3f background;

	SceneSettings()
		: width(SCENE_DEFAULT_WIDTH),
		height(SCENE_DEFAULT_HEIGHT),
		subdivs(SCENE_DEFAULT_SUBDIVS),
		background(SCENE_DEFAULT_BACKGROUND) {}

	void parse(std::unordered_map<std::string, std::string>& map) override {
		SceneParser& parser = getParser();
		parser.parseuint32AndStore(map, "width", this->width);
		parser.parseuint32AndStore(map, "height", this->height);
		parser.parseuint32AndStore(map, "subdivs", this->subdivs);
		parser.parseVector3fAndStore(map, "background", this->background);
	}
};
