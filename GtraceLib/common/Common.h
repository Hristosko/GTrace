#pragma once

#include <memory>
#include <string_view>

namespace gtrace
{
template<typename To, typename From>
std::unique_ptr<To> staticCastPointer(std::unique_ptr<From>&& from)
{
    From* fromPtr = from.release();
    return std::unique_ptr<To>(static_cast<To*>(fromPtr));
}

std::string_view getFileName(const char* fullPath);

}  // namespace gtrace
