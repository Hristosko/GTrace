#include "File.h"
#include "common/Errors.h"

namespace gtrace
{
File::~File()
{
    close();
}

File::File(File&& rhs) : fp(rhs.fp)
{
    rhs.fp = nullptr;
}

File& File::operator=(File&& rhs)
{
    fp = rhs.fp;
    rhs.fp = nullptr;
    return *this;
}

bool File::isAtEnd() const
{
    return /*!isOpen() || */ feof(fp);
}

bool File::isOpen() const
{
    return fp != nullptr;
}

void File::close()
{
    if (fp != nullptr)
    {
        fclose(fp);
        fp = nullptr;
    }
}

}  // namespace gtrace