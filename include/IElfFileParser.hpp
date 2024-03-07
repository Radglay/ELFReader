#pragma once

#include <fstream>
#include "FileHeader.hpp"


namespace parser
{
class IFileHeaderParser;

class IElfFileParser
{
public:
    virtual ~IElfFileParser() = default;
    virtual FileHeader parseFileHeader() = 0;
    virtual IFileHeaderParser* createFileHeaderParser() = 0;
};

}

