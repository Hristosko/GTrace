#pragma once

#include <unordered_map>
#include <string>
#include <memory>

#include "math/Vector3f.h"

namespace gtrace {

class Texture;
class Material;
class World;

class SceneParser;
class SceneElement {
public:
    virtual ~SceneElement() {}
    virtual void parse(const SceneParser& parser, std::unordered_map<std::string, std::string>& map) = 0;
};

class SceneParser
{
public:
    SceneParser(World& w);
    void parseFile(const char* path);

    Vector3f parseVector3f(const std::string& str) const;
    uint32_t parseuint32(const std::string& str) const;
    float parsefloat(const std::string& str) const;

    void parseVector3fAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Vector3f& res) const;
    void parseuint32AndStore(std::unordered_map<std::string, std::string>& map, const char* name, uint32_t& res) const;
    void parsefloatAndStore(std::unordered_map<std::string, std::string>& map, const char* name, float& res) const;
    void parseTextureAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Texture*& res) const;
    void parseMaterialAndStore(std::unordered_map<std::string, std::string>& map, const char* name, Material*& res) const;
    void parseBoolAndStore(std::unordered_map<std::string, std::string>& map, const char* name, bool& res) const;

    World& getWorld() const { return this->world; }

protected:
    void makeElement(World& w, const std::string& obj, std::unordered_map<std::string, std::string>& fields);
    SceneElement* getByName(const std::string& name);

    typedef std::unique_ptr<SceneElement>(*ElementCreator)();
private:
    World& world;
    /// Store the current object name in global variable
    /// so that we are able to log in warnings, error etc.
    uint32_t curObjLine;
    std::string curObject;
    /** map the names of the objects in the scene file to functions that will create object of the specified type */
    const std::unordered_map<std::string, ElementCreator> elementFactory;
};
}
