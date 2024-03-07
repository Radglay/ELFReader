#pragma once

#include "ISectionHeaderParser.hpp"
#include "SectionHeader.hpp"


namespace parser
{

class SectionHeaderParserX32 : public ISectionHeaderParser
{
public:
    SectionHeader parse(const char*) override;
};

}
