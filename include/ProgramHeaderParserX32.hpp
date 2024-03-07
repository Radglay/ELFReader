#pragma once

#include "IProgramHeaderParser.hpp"
#include "ProgramHeader.hpp"


namespace parser
{

class ProgramHeaderParserX32 : public IProgramHeaderParser
{
public:
    ProgramHeader parse(const char*) override;
};

}
