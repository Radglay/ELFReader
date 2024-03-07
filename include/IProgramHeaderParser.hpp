#pragma once

#include <elf.h>
#include "ProgramHeader.hpp"


namespace parser
{

class IProgramHeaderParser
{
public:
    virtual ~IProgramHeaderParser() = default;
    virtual ProgramHeader parse(const char*) = 0;
};

}
