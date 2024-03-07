#pragma once

#include "IProgramHeaderParser.hpp"
#include "ProgramHeader.hpp"


namespace parser
{

class ProgramHeaderParserX64 : public IProgramHeaderParser
{
public:
    ProgramHeader parse(const char*) override;
};

}
