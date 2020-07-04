#pragma once

#include<unordered_map>
#include <string>

#include "../math/Vector3f.h"

class SceneParser;

class SceneElement {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) = 0;
};

class SceneParser {
public:
	typedef void(*ElementParseFunc)(std::unordered_map<std::string, std::string>);

	void parseFile(const char* path);
	void addElement(const char* name, ElementParseFunc func);

	Vector3f parseVector3f(const std::string& str) const;
	uint32_t parseuint32(const std::string& str) const;
	
	void parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3f& res) const;
	void parseuint32AndStore(std::unordered_map<std::string, std::string>& map, const char* name, uint32_t& res) const;

private:
	std::unordered_map<std::string, ElementParseFunc> elementParsers;
};

SceneParser& getParser();