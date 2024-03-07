#pragma once

#include "IFileHeaderParser.hpp"
#include "FileHeader.hpp"


namespace parser
{

class FileHeaderParserX32 : public IFileHeaderParser
{
public:
    FileHeader parse(const char*) override;
};

}
