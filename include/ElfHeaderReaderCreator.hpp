#pragma once

#include <istream>


namespace reader
{

class IElfHeaderReader;


class ElfHeaderReaderCreator
{
public:
    static IElfHeaderReader* createElfHeaderReader(std::istream*);
};

}