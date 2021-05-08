#include "Parser.h"
#include "../Logger.h"
#include "../Errors.h"
#include "../FileUtils.h"
#include "World.h"

#define DELIMER '='
#define COMMENT_START '#'
#define MULTILINE_COMMENT '@'
#define MAX_LINE_LENGTH 200

#define FALSE_STRING "false"
#define TRUE_STRING "true"

#include "../geometry/Triangle.h"
#include "../geometry/Sphere.h"
#include "../geometry/Mesh.h"
#include "../texture/ColorTexture.h"
#include "../texture/NoiseTexture.h"
#include "../material/MatteMaterial.h"
#include "../light/DirectionalLight.h"
#include "../camera/PinholeCamera.h"
#include "../camera/OrthogonalCamera.h"

namespace gtrace {

SceneElement* SceneParser::getByName(const std::string& name) {
	if (name == "Triangle")
		return new Triangle();
	if (name == "Sphere")
		return new Sphere();
	if (name == "Mesh")
		return new Mesh();
	if (name == "ColorTexture")
		return new ColorTexture();
	if (name == "NoiseTexture")
		return new NoiseTexture();
	if (name == "MatteMaterial")
		return new MatteMaterial();
	if (name == "DirectionalLight")
		return new DirectionalLight();
	if (name == "PinholeCamera")
		return new PinholeCamera();
	if (name == "OrthogonalCamera")
		return new OrthogonalCamera();
	if (name == "Settings")
		return &this->world.getSettings();

	LOGERROR("Parsing unknown scene element: ", name, " length: ", name.size());
	throw ParseError();
}

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

void SceneParser::makeElement(World& w, const std::string& obj, std::unordered_map<std::string, std::string>& fields) {
	SceneElement* el = getByName(obj);
	el->parse(*this, fields);
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
	int res = sscanf_s(str.c_str(), "(%f, %f, %f)", &x, &y, &z);
	if (res < 3) {
		LOGERROR("Parsing Vector3f: unknown format; line: ", curObjLine);
		throw ParseError();
	}
	return Vector3f(x, y, z);
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
