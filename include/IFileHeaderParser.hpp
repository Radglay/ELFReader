#pragma once

#include <elf.h>
#include "FileHeader.hpp"


namespace parser
{

class IFileHeaderParser
{
public:
    virtual ~IFileHeaderParser() = default;
    virtual FileHeader parse(const char*) = 0;
};

}
