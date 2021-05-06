#pragma once

struct GTraceError {};

struct ParseError : GTraceError {};

struct FileError : GTraceError {};

struct FileWriteError : FileError {};

struct FileReadError: FileError {};