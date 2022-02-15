#pragma once

namespace gtrace
{
template<typename BoundingVolume>
class Bounded
{
public:
    virtual BoundingVolume bound() const = 0;
};

}