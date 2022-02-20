#include "FileWriter.h"
#include "common/Errors.h"

namespace gtrace
{
FileWriter::FileWriter(const char* path) : File(fopen(path, "w"))
{
    if (!isOpen())
        Raise(FileError(std::string("Cannot open file: ") + path));
}

void FileWriter::writeLine(const char* data) const
{
    if (!isOpen())
        Raise(FileError("Write to unopen file."));
    fprintf(fp, "%s\n", data);
}

BinFileWriter::BinFileWriter(const char* path) : File(fopen(path, "wb"))
{
    if (!isOpen())
        Raise(FileError(std::string("Cannot open file: ") + path));
}

void BinFileWriter::write(const void* data, size_t size, size_t count) const
{
    if (!isOpen())
        Raise(FileError("Write to unopen file."));

    const auto res = fwrite(data, size, count, fp);
    if (res != count)
        Raise(FileError(
            "Unable to write all requested data to file: " + std::to_string(res) + " from " + std::to_string(count)));
}

}  // namespace gtrace
