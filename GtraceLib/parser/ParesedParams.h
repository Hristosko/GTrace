#pragma once
#include <string>
#include <unordered_map>

namespace gtrace
{
class Vector3f;
class ParsedParams
{
private:
    template<typename T>
    using Params = std::unordered_map<std::string, T>;
    struct Vector3fStorage
    {
        float x;
        float y;
        float z;
    };

public:
    float getFloat(const std::string& name) const;
    int getInt(const std::string& name) const;
    Vector3f getVector3f(const std::string& name) const;

    void addFloat(const std::string& name, float value);
    void addInt(const std::string& name, int value);
    void addVector3f(const std::string& name, float x, float y, float z);

private:
    Params<float> floats;
    Params<int> ints;
    Params<Vector3fStorage> vector3fs;
};

}  // namespace gtrace