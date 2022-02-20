#include "RendererOutput.h"
#include "common/Errors.h"

namespace gtrace
{
void RendererOutput::addOutput(const std::string& name, DataBuffer&& data)
{
    const auto[_, inserted] = outputs.insert(std::make_pair(name, std::move(data)));
    if (!inserted)
        Raise(DuplicatedError("Rederer output " + name + " allready exists."));
}

DataBuffer& RendererOutput::getOutput(const std::string& name)
{
    const auto it = outputs.find(name);
    if (it == outputs.end())
        Raise(NotFoundError("Rederer output " + name + " doesn't exist."));
    return it->second;
}

const DataBuffer& RendererOutput::getOutput(const std::string& name) const
{
    const auto it = outputs.find(name);
    if (it == outputs.end())
        Raise(NotFoundError("Rederer output " + name + " doesn't exist."));
    return it->second;
}

}  // namespace gtrace