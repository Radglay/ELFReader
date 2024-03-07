#pragma once

#include "IFileHeaderParser.hpp"
#include "FileHeader.hpp"


namespace parser
{

class FileHeaderParserX64 : public IFileHeaderParser
{
public:
    FileHeader parse(const char*) override;
};

}
