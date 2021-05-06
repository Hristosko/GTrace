#pragma once

#include<unordered_map>
#include <string>

#include "../math/Vector3f.h"

class Texture;
class Material;

class SceneElement {
public:
	virtual void parse(std::unordered_map<std::string, std::string>& map) = 0;
};

namespace SceneParser
{
	void parseFile(const char* path);

	Vector3f parseVector3f(const std::string& str);
	uint32_t parseuint32(const std::string& str);
	float parsefloat(const std::string& str);
	
	void parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3f& res);
	void parseuint32AndStore(std::unordered_map<std::string, std::string>& map, const char* name, uint32_t& res);
	void parsefloatAndStore(std::unordered_map<std::string, std::string>& map, const char* name, float& res);
	void parseTextureAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Texture*& res);
	void parseMaterialAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Material*& res);
	void parseBoolAndStore(std::unordered_map<std::string, std::string>& map, const char* name, bool& res);
};
