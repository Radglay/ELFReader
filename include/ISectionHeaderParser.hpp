#pragma once

#include "SectionHeader.hpp"


namespace parser
{

class ISectionHeaderParser
{
public:
    virtual ~ISectionHeaderParser() = default;
    virtual SectionHeader parse(const char*) = 0;
};

}
