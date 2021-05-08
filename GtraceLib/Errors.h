#pragma once

namespace gtrace {

struct GTraceError {};

struct ParseError : GTraceError {};

struct FileError : GTraceError {};

struct FileWriteError : FileError {};

struct FileReadError: FileError {};

}
