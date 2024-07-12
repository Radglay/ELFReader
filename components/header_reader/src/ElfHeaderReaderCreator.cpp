#include "ElfHeaderReaderCreator.hpp"
#include "IElfHeaderReader.hpp"
#include "CheckingFileVersionUtility.hpp"
#include <istream>
#include "ElfHeaderReaderX32.hpp"
#include "ElfHeaderReaderX64.hpp"


namespace reader
{

IElfHeaderReader* ElfHeaderReaderCreator::createElfHeaderReader(std::istream* p_fileStream)
{
    if (is32BitVersion(*p_fileStream))
    {
        return new ElfHeaderReaderX32(p_fileStream);
    }
    else if (is64BitVersion(*p_fileStream))
    {
        return new ElfHeaderReaderX64(p_fileStream);
    }

    return nullptr;
}


}