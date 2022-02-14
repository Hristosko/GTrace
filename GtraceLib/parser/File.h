#pragma once
#include <stdio.h>
#include "common/NonCopyable.h"

namespace gtrace
{
class File : public NonCopyable
{
public:
    explicit File(FILE* fp = nullptr) : fp(fp) {}
    File(File&& rhs);
    File& operator=(File&& rhs);
    ~File();

    bool isAtEnd() const;
    bool isOpen() const;
    void close();

protected:
    FILE* fp;
};

}  // namespace gtrace