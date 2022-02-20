#include <assert.h>
#include "FileReader.h"
#include "common/Errors.h"

namespace gtrace
{
FileReader::FileReader(const char* path) : File(fopen(path, "r"))
{
    if (!isOpen())
        Raise(FileError(std::string("Cannot open file: ") + path));
}

bool FileReader::readLine(std::string* res) const
{
    if (!isOpen())
        Raise(FileError("Read from unopen file."));

    bool extraData = false;

    *res = {};
    for (;;)
    {
        if (nullptr == fgets(buffer, maxLineLength, fp))
            return extraData;

        *res += buffer;
        if (res->back() == '\n')
        {
            res->pop_back();
            if (res->back() == '\r')
                res->pop_back();
            return true;
        }
        else
            extraData = true;
    }

    assert(false);
    return false;
}

BinFileReader::BinFileReader(const char* path) : File(fopen(path, "rb"))
{
    if (!isOpen())
        Raise(FileError(std::string("Cannot open file: ") + path));
}

void BinFileReader::read(void* buffer, size_t size, size_t count) const
{
    if (!isOpen())
        Raise(FileError("Read from unopen file."));

    const auto res = fread(buffer, size, count, fp);
    if (res != count)
        Raise(FileError(
            "Unable to read all requested data from file: " + std::to_string(res) + " from " + std::to_string(count)));
}

}  // namespace gtrace
