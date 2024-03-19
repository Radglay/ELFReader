#pragma once
#include <istream>


namespace parser
{

class IElfFileParser;


class ElfFileParserCreator
{
public:
    static IElfFileParser* createElfFileParser(std::istream&);
};

}