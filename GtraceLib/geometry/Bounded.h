#pragma once

namespace gtrace
{
template<typename BoundingVolume>
class Bounded
{
public:
    virtual ~Bounded() = default;
    virtual BoundingVolume bound() const = 0;
};

}  // namespace gtrace