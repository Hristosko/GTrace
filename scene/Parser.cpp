#include "Parser.h"
#include "../Logger.h"
#include "../Errors.h"
#include "World.h"

#include <fstream>

#define DELIMER '='

#include "../geometry/Triangle.h"
#include "../geometry/Sphere.h"
#include "../texture/ColorTexture.h"

static SceneElement* getByName(const std::string& name) {
	if (name == "Triangle")
		return new Triangle();
	if (name == "Sphere")
		return new Sphere();
	if (name == "ColorTexture")
		return new ColorTexture();
	if (name == "Camera")
		return &getWorld().getCamera();
	if (name == "Settings")
		return &getWorld().getSettings();

	LOGERROR("Parsing unknown scene element: ", name);
	throw ParseError();
}

void split(const std::string& src, std::string& a, std::string& b) {
	std::string* cur = &a;
	for (char c : src) {
		if (c == DELIMER && cur == &a) {
			cur = &b;
		}
		else {
			cur->push_back(c);
		}
	}
}

static void makeElement(const std::string& obj, std::unordered_map<std::string, std::string>& fields) {
	World& w = getWorld();
	SceneElement* el = getByName(obj);
	el->parse(fields);
	w.addElemenet(el, fields);
	fields.clear();
}

void SceneParser::parseFile(const char* path) {
	std::ifstream file(path);

	std::unordered_map<std::string, std::string> fields;
	std::string line;
	std::string curObject;
	bool atObject = false;
	while (std::getline(file, line)) {
		if (!atObject) {
			curObject = line;
			if (curObject != "") {
				atObject = true;
			}
		}
		else {
			if (line == "") {
				makeElement(curObject, fields);
				atObject = false;
				continue;
			}
			std::string a, b;
			split(line, a, b);
			fields.insert(std::make_pair(std::move(a), std::move(b)));
		}
	}
	if (atObject) makeElement(curObject, fields);
}

void SceneParser::addElement(const char* name, ElementParseFunc func) {
	elementParsers[std::string(name)] = func;
}

Vector3f SceneParser::parseVector3f(const std::string& str) const {
	if (str[0] != '(') {
		LOGERROR("Parsing Vector3f: expected (");
		throw ParseError();
	}
	if (str[str.size()-1] != ')') {
		LOGERROR("Parsing Vector3f: expected )");
		throw ParseError();
	}
	float x, y, z;
	int res = sscanf_s(str.c_str(), "(%f, %f, %f)", &x, &y, &z);
	if (res < 3) {
		LOGERROR("Parsing Vector3f: unknown format");
		throw ParseError();
	}
	return Vector3f(x, y, z);
}

uint32_t SceneParser::parseuint32(const std::string& str) const {
	uint32_t x;
	int res = sscanf_s(str.c_str(), "%lo", &x);
	if (res < 1) {
		LOGERROR("Parsing uint32_t failed");
		throw ParseError();
	}
	return x;
}

float SceneParser::parsefloat(const std::string& str) const {
	float x;
	int res = sscanf_s(str.c_str(), "%f", &x);
	if (res < 1) {
		LOGERROR("Parsing float failed");
		throw ParseError();
	}
	return x;
}

void SceneParser::parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3f& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGINFO("Missing parameter: ", name);
		return;
	}
	res = this->parseVector3f(it->second);
}

void SceneParser::parseuint32AndStore(std::unordered_map<std::string, std::string>& map, const char* name, uint32_t& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGINFO("Missing parameter: ", name);
		return;
	}
	res = this->parseuint32(it->second);
}

void SceneParser::parsefloatAndStore(std::unordered_map<std::string, std::string>& map, const char* name, float& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGINFO("Missing parameter: ", name);
		return;
	}
	res = this->parsefloat(it->second);
}

void SceneParser::parseTextureAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Texture*& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGINFO("Missning texture: ", name);
		res = nullptr;
	}
	else res = getWorld().getTextureByName(it->second);
}

SceneParser& getParser() {
	static SceneParser parser;
	return parser;
}
