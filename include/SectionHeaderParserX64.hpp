#pragma once

#include "ISectionHeaderParser.hpp"
#include "SectionHeader.hpp"


namespace parser
{

class SectionHeaderParserX64 : public ISectionHeaderParser
{
public:
    SectionHeader parse(const char*) override;
};

}
