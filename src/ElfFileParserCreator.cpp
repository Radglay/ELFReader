#include "ElfFileParserCreator.hpp"
#include "IElfFileParser.hpp"
#include "CheckingFileVersionUtility.hpp"
#include <fstream>
#include "ElfFileParserX32.hpp"
#include "ElfFileParserX64.hpp"


namespace parser
{

IElfFileParser* ElfFileParserCreator::createElfFileParser(const char* p_fileName)
{
    std::ifstream l_fileStream { p_fileName, std::ios::binary };

    if (is32BitVersion(l_fileStream))
    {
        return new ElfFileParserX32(std::move(l_fileStream));
    }
    else if (is64BitVersion(l_fileStream))
    {
        return new ElfFileParserX64(std::move(l_fileStream));
    }

    return nullptr;
}


}