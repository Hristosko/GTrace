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

bool FileReader::readLine(std::string* res)
{
    if (!isOpen())
        Raise(FileError("Read to unopen file."));

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
}  // namespace gtrace
