#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include "math/Matrix4x4.h"

namespace gtrace
{
class Vector3f;
class Matrix4x4;
class Transform;
class ParsedParams
{
private:
    template<typename T>
    using Params = std::unordered_map<std::string, T>;

public:
    float getFloat(const std::string& name) const;
    int getInt(const std::string& name) const;
    Vector3f getVector3f(const std::string& name) const;
    const std::string& getString(const std::string& name) const;

    void addFloat(const std::string& name, float value);
    void addInt(const std::string& name, int value);
    void addVector3f(const std::string& name, float x, float y, float z);
    void addVector3f(const std::string& name, const Vector3f& vec);
    void addString(const std::string& name, const std::string& value);

    std::shared_ptr<Transform> getTransform() const;
    void addTranslation(const Vector3f& vec);
    void addRotation(float x, float y, float z);
    void addScale(float scalar);

private:
    Params<float> floats;
    Params<int> ints;
    Params<Vector3f> vector3fs;
    Params<std::string> strings;
    Matrix4x4 transformation;
    mutable std::shared_ptr<Transform> transform;
};

}  // namespace gtrace