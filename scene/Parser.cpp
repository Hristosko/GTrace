#include "Parser.h"
#include "../Logger.h"
#include "../Errors.h"
#include "../FileUtils.h"
#include "World.h"

#include <fstream>

#define DELIMER '='
#define MAX_LINE_LENGTH 200

#include "../geometry/Triangle.h"
#include "../geometry/Sphere.h"
#include "../texture/ColorTexture.h"
#include "../texture/NoiseTexture.h"
#include "../material/MatteMaterial.h"
#include "../light/DirectionalLight.h"

/// Store the current object name in global variable
/// so that we are able to log in warnings, error etc.
static std::string curObject;
static uint32_t curObjLine;

static SceneElement* getByName(const std::string& name) {
	if (name == "Triangle")
		return new Triangle();
	if (name == "Sphere")
		return new Sphere();
	if (name == "ColorTexture")
		return new ColorTexture();
	if (name == "NoiseTexture")
		return new NoiseTexture();
	if (name == "MatteMaterial")
		return new MatteMaterial();
	if (name == "DirectionalLight")
		return new DirectionalLight();
	if (name == "Camera")
		return &getWorld().getCamera();
	if (name == "Settings")
		return &getWorld().getSettings();

	LOGERROR("Parsing unknown scene element: ", name, " length: ", name.size());
	throw ParseError();
}

void split(const char* src, size_t len, std::string& a, std::string& b) {
	std::string* cur = &a;
	for (size_t i = 0; i < len; ++i) {
		const char c = src[i];
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
	FILE* fp;
	if (0 != fopen_s(&fp, path, "r")) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	FileRAII fpRAII(fp);

	char buffer[MAX_LINE_LENGTH];
	std::unordered_map<std::string, std::string> fields;
	uint32_t line = 0;
	bool atObject = false;

	int len;
	while ((len = readLine(buffer, MAX_LINE_LENGTH, fp)) >= 0) {
		++line;
		if (!atObject) {
			if (len != 0) {
				atObject = true;
				curObject = buffer;
				curObjLine = line;
			}
		}
		else {
			if (len == 0) {
				makeElement(curObject, fields);
				atObject = false;
				continue;
			}
			std::string a, b;
			split(buffer, len, a, b);
			fields.insert(std::make_pair(std::move(a), std::move(b)));
		}
	}
	if (atObject) makeElement(curObject, fields);
}

void SceneParser::addElement(const char* name, ElementParseFunc func) {
	elementParsers[std::string(name)] = func;
}

Vector3fData SceneParser::parseVector3f(const std::string& str) const {
	if (str[0] != '(') {
		LOGERROR("Parsing Vector3f: expected (; line: ", curObjLine);
		throw ParseError();
	}
	if (str[str.size()-1] != ')') {
		LOGERROR("Parsing Vector3f: expected ); line: ", curObjLine);
		throw ParseError();
	}
	float x, y, z;
	int res = sscanf_s(str.c_str(), "(%f, %f, %f)", &x, &y, &z);
	if (res < 3) {
		LOGERROR("Parsing Vector3f: unknown format; line: ", curObjLine);
		throw ParseError();
	}
	return Vector3fData(x, y, z);
}

uint32_t SceneParser::parseuint32(const std::string& str) const {
	uint32_t x;
	int res = sscanf_s(str.c_str(), "%lo", &x);
	if (res < 1) {
		LOGERROR("Parsing uint32_t failed; line: ", curObjLine);
		throw ParseError();
	}
	return x;
}

float SceneParser::parsefloat(const std::string& str) const {
	float x;
	int res = sscanf_s(str.c_str(), "%f", &x);
	if (res < 1) {
		LOGERROR("Parsing float failed; line: ", curObjLine);
		throw ParseError();
	}
	return x;
}

void SceneParser::parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3fData& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing parameter: ", name, " object: ", curObject, " line: ", curObjLine);
		return;
	}
	res = this->parseVector3f(it->second);
}

void SceneParser::parseuint32AndStore(std::unordered_map<std::string, std::string>& map, const char* name, uint32_t& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing parameter: ", name, " object: ", curObject, " line: ", curObjLine);
		return;
	}
	res = this->parseuint32(it->second);
}

void SceneParser::parsefloatAndStore(std::unordered_map<std::string, std::string>& map, const char* name, float& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing parameter: ", name, " object: ", curObject, " line: ", curObjLine);
		return;
	}
	res = this->parsefloat(it->second);
}

void SceneParser::parseTextureAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Texture*& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missning texture: ", name, " object: ", curObject, " line: ", curObjLine);
		res = nullptr;
	}
	else res = getWorld().getTextureByName(it->second);
}

void SceneParser::parseMaterialAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Material*& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missning texture: ", name, " object: ", curObject, " line: ", curObjLine);
		res = nullptr;
	}
	else res = getWorld().getMaterialByName(it->second);
}

SceneParser& getParser() {
	static SceneParser parser;
	return parser;
}
