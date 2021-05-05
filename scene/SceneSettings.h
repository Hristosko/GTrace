#pragma once

#include<cstdint>
#include "Parser.h"

#define SCENE_DEFAULT_WIDTH 800
#define SCENE_DEFAULT_HEIGHT 600
#define SCENE_DEFAULT_MAX_SUBDIVS 4
#define SCENE_DEFAULT_STD_TRESHHOLD 0.05f

#define SCENE_DEFAULT_BACKGROUND Vector3f(0.f)

struct SceneSettings : public SceneElement {
	uint32_t width;
	uint32_t height;
	uint32_t maxSubdivs;
	float stdTreshhold;
	Material* background;

	SceneSettings()
		: width(SCENE_DEFAULT_WIDTH),
		height(SCENE_DEFAULT_HEIGHT),
		maxSubdivs(SCENE_DEFAULT_MAX_SUBDIVS),
		stdTreshhold(SCENE_DEFAULT_STD_TRESHHOLD),
		background(nullptr) {}

	void parse(std::unordered_map<std::string, std::string>& map) override {
		SceneParser::parseuint32AndStore(map, "width", this->width);
		SceneParser::parseuint32AndStore(map, "height", this->height);
		SceneParser::parseuint32AndStore(map, "max_subdivs", this->maxSubdivs);
		SceneParser::parsefloatAndStore(map, "std_treshhold", this->stdTreshhold);
		SceneParser::parseMaterialAndStore(map, "background", this->background);
	}
};
