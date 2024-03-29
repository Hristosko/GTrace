#include "ParsedParams.h"
#include "common/Errors.h"
#include "math/Vector3f.h"
#include "math/Transform.h"

namespace gtrace
{
static void logOverwritten(bool isInserted, const std::string& name)
{
    if (!isInserted)
        LOGWARNING("Parameter ", name, "is overwritten");
}

float ParsedParams::getFloat(const std::string& name) const
{
    const auto it = floats.find(name);
    if (it == floats.end())
        Raise(ParserError("Parameter not found: " + name));
    return it->second;
}

int ParsedParams::getInt(const std::string& name) const
{
    const auto it = ints.find(name);
    if (it == ints.end())
        Raise(ParserError("Parameter not found: " + name));
    return it->second;
}

Vector3f ParsedParams::getVector3f(const std::string& name) const
{
    const auto it = vector3fs.find(name);
    if (it == vector3fs.end())
        Raise(ParserError("Parameter not found: " + name));
    return it->second;
}

const std::string& ParsedParams::getString(const std::string& name) const
{
    const auto it = strings.find(name);
    if (it == strings.end())
        Raise(ParserError("Parameter not found: " + name));
    return it->second;
}

void ParsedParams::addFloat(const std::string& name, float value)
{
    const auto [_, inserted] = floats.insert_or_assign(name, value);
    logOverwritten(inserted, name);
}

void ParsedParams::addInt(const std::string& name, int value)
{
    const auto [_, inserted] = ints.insert_or_assign(name, value);
    logOverwritten(inserted, name);
}

void ParsedParams::addVector3f(const std::string& name, float x, float y, float z)
{
    addVector3f(name, Vector3f(x, y, z));
}

void ParsedParams::addVector3f(const std::string& name, const Vector3f& vec)
{
    const auto [_, inserted] = vector3fs.insert_or_assign(name, vec);
    logOverwritten(inserted, name);
}

void ParsedParams::addString(const std::string& name, const std::string& value)
{
    const auto [_, inserted] = strings.insert_or_assign(name, value);
    logOverwritten(inserted, name);
}

std::shared_ptr<Transform> ParsedParams::getTransform() const
{
    if (!transform)
        transform = std::make_shared<Transform>(transformation);
    return transform;
}

void ParsedParams::addTranslation(const Vector3f& vec)
{
    transformation *= Transform::makeTranslation(vec);
}

void ParsedParams::addRotation(float x, float y, float z)
{
    transformation *= Transform::makeRotation(x, y, z);
}

void ParsedParams::addScale(float scalar)
{
    transformation *= Transform::makeScale(scalar);
}

}  // namespace gtrace