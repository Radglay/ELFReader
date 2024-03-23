#include "ElfFileParserCreator.hpp"
#include "IElfFileParser.hpp"
#include "CheckingFileVersionUtility.hpp"
#include <istream>
#include "ElfFileParserX32.hpp"
#include "ElfFileParserX64.hpp"


namespace parser
{

IElfFileParser* ElfFileParserCreator::createElfFileParser(std::istream* p_fileStream)
{
    if (is32BitVersion(*p_fileStream))
    {
        return new ElfFileParserX32(p_fileStream);
    }
    else if (is64BitVersion(*p_fileStream))
    {
        return new ElfFileParserX64(p_fileStream);
    }

    return nullptr;
}


}