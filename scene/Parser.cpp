#include "Parser.h"
#include "../Logger.h"
#include "../Errors.h"
#include "World.h"

#include <fstream>

#define DELIMER '='

#include "../geometry/Triangle.h"

static SceneElement* getByName(const std::string& name) {
	if (name == "Triangle")
		return new Triangle();

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
	w.addElemenet(el);
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

void SceneParser::parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3f& res) const {
	auto it = map.find(name);
	if (it == map.end()) {
		LOGINFO("Missing parameter: ", name);
		return;
	}
	res = this->parseVector3f(it->second);
}

SceneParser& getParser() {
	static SceneParser parser;
	return parser;
}
