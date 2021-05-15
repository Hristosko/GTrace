#include "Parser.h"
#include "Logger.h"
#include "Errors.h"
#include "FileUtils.h"
#include "World.h"

#define DELIMER '='
#define COMMENT_START '#'
#define MULTILINE_COMMENT '@'
#define MAX_LINE_LENGTH 200

#define FALSE_STRING "false"
#define TRUE_STRING "true"

#include "geometry/Triangle.h"
#include "geometry/Sphere.h"
#include "geometry/Mesh.h"
#include "texture/ColorTexture.h"
#include "texture/NoiseTexture.h"
#include "material/MatteMaterial.h"
#include "light/DirectionalLight.h"
#include "camera/PinholeCamera.h"
#include "camera/OrthogonalCamera.h"

namespace gtrace {

static void split(const char* src, size_t len, std::string& a, std::string& b) {
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


/**
* Utility function and macros for creating the elementFactory.
*/
template<typename T>
std::unique_ptr<SceneElement> createElement() {
	return std::make_unique<T>();
}

#define ElementCreatorMappedCustomName(Name, TypeName) { #Name, createElement<TypeName> }
#define ElementCreatorMapped(Name) ElementCreatorMappedCustomName(Name, Name)

SceneParser::SceneParser(World& w)
	: world(w), curObjLine(0),
	elementFactory {
		ElementCreatorMapped(Triangle),
		ElementCreatorMapped(Sphere),
		ElementCreatorMapped(Mesh),
		ElementCreatorMapped(ColorTexture),
		ElementCreatorMapped(NoiseTexture),
		ElementCreatorMapped(MatteMaterial),
		ElementCreatorMapped(DirectionalLight),
		ElementCreatorMapped(PinholeCamera),
		ElementCreatorMapped(OrthogonalCamera),
		ElementCreatorMappedCustomName(Settings, SceneSettings),
	} {}

void SceneParser::makeElement(World& w, const std::string& obj, std::unordered_map<std::string, std::string>& fields) {
	auto it = this->elementFactory.find(obj);
	if (it == this->elementFactory.cend()) {
		LOGERROR("Parsing unknown scene element: ", obj, " length: ", obj.size());
		throw ParseError();
	}
	std::unique_ptr<SceneElement> el = it->second();
	el->parse(*this, fields);
	w.addElemenet(std::move(el), fields);
	fields.clear();
}

void SceneParser::parseFile(const char* path) {
	FILE* fp = fopen(path, "r");
	if (fp == nullptr) {
		LOGERROR("Cannot open file: ", path);
		throw FileError();
	}
	FileRAII fpRAII(fp);

	char buffer[MAX_LINE_LENGTH];
	std::unordered_map<std::string, std::string> fields;
	uint32_t line = 0;
	bool atObject = false;
	bool insideComment = false;

	int len;
	while ((len = readLine(buffer, MAX_LINE_LENGTH, fp)) >= 0) {
		++line;
		if (buffer[0] == COMMENT_START) continue;
		if (insideComment == false && buffer[0] == MULTILINE_COMMENT)
		{
			insideComment = true;
			continue;
		}
		if (insideComment)
		{
			if (buffer[len - 1] == MULTILINE_COMMENT) insideComment = false;
			continue;
		}
		if (!atObject) {
			if (len != 0) {
				atObject = true;
				curObject = buffer;
				curObjLine = line;
			}
		}
		else {
			if (len == 0) {
				makeElement(world, curObject, fields);
				atObject = false;
				continue;
			}
			std::string a, b;
			split(buffer, len, a, b);
			fields.insert(std::make_pair(std::move(a), std::move(b)));
		}
	}
	if (atObject) makeElement(world, curObject, fields);
}

Vector3f SceneParser::parseVector3f(const std::string& str) const {
	if (str[0] != '(') {
		LOGERROR("Parsing Vector3f: expected (; line: ", curObjLine);
		throw ParseError();
	}
	if (str[str.size()-1] != ')') {
		LOGERROR("Parsing Vector3f: expected ); line: ", curObjLine);
		throw ParseError();
	}
	float x, y, z;
	int res = sscanf(str.c_str(), "(%f, %f, %f)", &x, &y, &z);
	if (res < 3) {
		LOGERROR("Parsing Vector3f: unknown format; line: ", curObjLine);
		throw ParseError();
	}
	return Vector3f(x, y, z);
}

uint32_t SceneParser::parseuint32(const std::string& str) const {
	uint32_t x;
	int res = sscanf(str.c_str(), "%u", &x);
	if (res < 1) {
		LOGERROR("Parsing uint32_t failed; line: ", curObjLine);
		throw ParseError();
	}
	return x;
}

float SceneParser::parsefloat(const std::string& str) const {
	float x;
	int res = sscanf(str.c_str(), "%f", &x);
	if (res < 1) {
		LOGERROR("Parsing float failed; line: ", curObjLine);
		throw ParseError();
	}
	return x;
}

void SceneParser::parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3f& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing parameter: ", name, " object: ", curObject, " line: ", curObjLine);
		return;
	}
	res = parseVector3f(it->second);
}

void SceneParser::parseuint32AndStore(std::unordered_map<std::string, std::string>& map, const char* name, uint32_t& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing parameter: ", name, " object: ", curObject, " line: ", curObjLine);
		return;
	}
	res = parseuint32(it->second);
}

void SceneParser::parsefloatAndStore(std::unordered_map<std::string, std::string>& map, const char* name, float& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing parameter: ", name, " object: ", curObject, " line: ", curObjLine);
		return;
	}
	res = parsefloat(it->second);
}

void SceneParser:: parseTextureAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Texture*& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing texture: ", name, " object: ", curObject, " line: ", curObjLine);
		res = nullptr;
	}
	else res = this->world.getTextureByName(it->second);
}

void SceneParser::parseMaterialAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Material*& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing texture: ", name, " object: ", curObject, " line: ", curObjLine);
		res = nullptr;
	}
	else res = this->world.getMaterialByName(it->second);
}

void SceneParser::parseBoolAndStore(std::unordered_map<std::string, std::string>& map, const char* name, bool& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGWARNING("Missing boolean: ", name, " object: ", curObject, " line: ", curObjLine, " Setting to default false");
		res = false;
	}
	else if (it->second == FALSE_STRING) res = false;
	else if (it->second == TRUE_STRING) res = true;
	else {
		LOGWARNING("Unknown boolean: ", it->second, " object: ", curObject, " line: ", curObjLine, " Setting to default false");
		res = false;
	}
}
}
