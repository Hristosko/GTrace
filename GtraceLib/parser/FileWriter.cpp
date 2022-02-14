#include "FileWriter.h"
#include "common/Errors.h"

namespace gtrace
{
FileWriter::FileWriter(const char* path) : File(fopen(path, "w"))
{
    if (!isOpen())
        Raise(FileError(std::string("Cannot open file: ") + path));
}

void FileWriter::writeLine(const char* data)
{
    if (!isOpen())
        Raise(FileError("Write to unopen file."));
    fprintf(fp, "%s\n", data);
}
}  // namespace gtrace
