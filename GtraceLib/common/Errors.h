#pragma once
#include <stdexcept>
#include <string>
#include "Logger.h"

namespace gtrace
{
struct GTraceError : std::runtime_error
{
    GTraceError(const std::string& message) : std::runtime_error(message) {}
};

struct InalidArgument : GTraceError
{
    InalidArgument(const std::string& message) : GTraceError(message) {}
};
struct ParserError : GTraceError
{
    ParserError(const std::string& message) : GTraceError(message) {}
};

struct FileError : GTraceError
{
    FileError(const std::string& message) : GTraceError(message) {}
};

struct NotFoundError : GTraceError
{
    NotFoundError(const std::string& message) : GTraceError(message) {}
};

struct DuplicatedError : GTraceError
{
    DuplicatedError(const std::string& message) : GTraceError(message) {}
};

struct InvalidValue : GTraceError
{
    InvalidValue(const std::string& message) : GTraceError(message) {}
};

struct InvalidDataBuffer : GTraceError
{
    InvalidDataBuffer(const std::string& message) : GTraceError(message) {}
};

#define Raise(error)            \
    {                           \
        LOGERROR(error.what()); \
        throw error;            \
    }
}  // namespace gtrace
